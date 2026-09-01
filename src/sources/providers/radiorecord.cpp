#include "radiorecord.h"
#include <QHttpMultiPart>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

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
        return;
    }

    Station station{tr("Radio Record - %0").arg(title),
                    rawStation.value(QStringLiteral("stream_hls")).toString(),
                    rawStation.value(QStringLiteral("icon_gray")).toString()};

    if (station.isValid()) {
        *stations << station;
    }
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

    m_runningReply = m_restAccessManager->post(
        m_api.createRequest(RadioRecordConstants::SearchPath), multiPart, this,
        &RadioRecord::onSearchRequestFinished);

    multiPart->setParent(m_runningReply);
}

void RadioRecord::handleLoadDefaultStations() {
    m_runningReply = m_restAccessManager->get(
        m_api.createRequest(RadioRecordConstants::DefaultStationsPath), this,
        &RadioRecord::onDefaultStationsRequestFinished);
}

bool RadioRecord::finishReply(QRestReply &reply, QJsonDocument *json) {
    m_runningReply = nullptr;

    if (!reply.isSuccess()) {
        raiseError(tr("Search error"), reply.networkReply()->errorString());

        return false;
    }

    return parseJson(reply, json);
}

void RadioRecord::handleStationsEndpointResult(const QJsonDocument &json) {
    const QJsonArray rawStations = extractStationsFromJson(json);
    QList<Station> stations;

    for (const QJsonValue &rawStation : rawStations) {
        processStationIntoList(rawStation.toObject(), &stations);
    }

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

    emit stationsDispatched(stations);
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
