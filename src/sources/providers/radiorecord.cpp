#include "radiorecord.h"
#include "../../logging/logcategories.h"
#include <QByteArray>
#include <QHttpMultiPart>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkRequest>

void RadioRecord::cancelSearch() {
    if (!m_runningReply) {
        return;
    }

    m_runningReply->abort();

    emit searchCancelled();
}

bool RadioRecord::hasDefaultStations() const {
    return true;
}

QString RadioRecord::websiteUrl() const {
    return RadioRecordConstants::WebsiteUrl.toString();
}

QJsonArray
RadioRecord::extractStationsFromJson(const QJsonDocument &json) const {
    return json.object()
        .value(QStringLiteral("result"))
        .toObject()
        .value(QStringLiteral("stations"))
        .toArray();
}

void RadioRecord::processStationIntoList(const QJsonObject &rawStation,
                                         QList<Station> *stations) const {
    if (rawStation.isEmpty()) {
        return;
    }

    const QString title = rawStation.value(QStringLiteral("title")).toString();

    if (title.isEmpty()) {
        qCDebug(lcSources) << "Skipping station without a title"
                           << rawStation.value(QStringLiteral("id"));

        return;
    }

    Station station{tr("Radio Record - %0").arg(title),
                    rawStation.value(QStringLiteral("stream_hls")).toString(),
                    rawStation.value(QStringLiteral("icon_gray")).toString()};

    if (!station.isValid()) {
        qCDebug(lcSources) << "Skipping" << title << "without a stream URL";

        return;
    }

    *stations << station;
}

void RadioRecord::handleSearch(const QString &query) {
    QHttpMultiPart *multiPart =
        new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart keywordsPart;
    keywordsPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                           QStringLiteral("form-data; name=\"keywords\""));
    keywordsPart.setBody(query.toUtf8());
    QHttpPart filtersPart;
    filtersPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                          QStringLiteral("form-data; name=\"filters[]\""));
    filtersPart.setBody(QByteArrayLiteral("stations"));

    multiPart->append(keywordsPart);
    multiPart->append(filtersPart);

    const QNetworkRequest request =
        m_api.createRequest(RadioRecordConstants::SearchPath);

    qCDebug(lcSources) << "POST" << request.url().toDisplayString()
                       << "keywords:" << query;

    m_runningReply = m_restAccessManager->post(
        request, multiPart, this, &RadioRecord::onSearchRequestFinished);

    multiPart->setParent(m_runningReply);
}

void RadioRecord::handleLoadDefaultStations() {
    const QNetworkRequest request =
        m_api.createRequest(RadioRecordConstants::DefaultStationsPath);

    qCDebug(lcSources) << "GET" << request.url().toDisplayString();

    m_runningReply = m_restAccessManager->get(
        request, this, &RadioRecord::onDefaultStationsRequestFinished);
}

void RadioRecord::handleStationsEndpointResult(const QJsonDocument &json) {
    const QJsonArray rawStations = extractStationsFromJson(json);
    QList<Station> stations;

    for (const QJsonValue &rawStation : rawStations) {
        processStationIntoList(rawStation.toObject(), &stations);
    }

    qCDebug(lcSources) << "Parsed" << stations.size() << "of"
                       << rawStations.size() << "default stations";

    if (stations.isEmpty()) {
        raiseError(tr("Search error"), tr("No default stations found"));

        return;
    }

    cacheDefaultStations(stations);

    emit stationsDispatched(stations);
}

void RadioRecord::handleSearchEndpointResult(const QJsonDocument &json) {
    const QJsonArray rawStations = extractStationsFromJson(json);
    QList<Station> stations;

    for (const QJsonValue &rawStation : rawStations) {
        processStationIntoList(rawStation.toObject(), &stations);
    }

    qCDebug(lcSources) << "Parsed" << stations.size() << "of"
                       << rawStations.size() << "found stations";

    emit stationsDispatched(stations);
}

bool RadioRecord::finishReply(QRestReply &reply, QJsonDocument *json) {
    m_runningReply = nullptr;

    const QString url = reply.networkReply()->url().toDisplayString();

    // aborted by cancelSearch(), which isn't an error
    if (reply.error() == QNetworkReply::OperationCanceledError) {
        qCDebug(lcSources) << "Request to" << url << "was cancelled";

        return false;
    }

    if (!reply.isSuccess()) {
        qCWarning(lcSources).nospace()
            << "Request to " << url << " failed (" << reply.error()
            << ", HTTP status " << reply.httpStatus()
            << "): " << reply.errorString();

        raiseError(tr("Search error"), reply.networkReply()->errorString());

        return false;
    }

    return parseJson(reply, json);
}

void RadioRecord::onSearchRequestFinished(QRestReply &reply) {
    QJsonDocument json;

    if (finishReply(reply, &json)) {
        handleSearchEndpointResult(json);
    }
}

void RadioRecord::onDefaultStationsRequestFinished(QRestReply &reply) {
    QJsonDocument json;

    if (finishReply(reply, &json)) {
        handleStationsEndpointResult(json);
    }
}
