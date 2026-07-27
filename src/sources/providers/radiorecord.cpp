#include "radiorecord.h"
#include "../../common/utilities.h"
#include <QHttpMultiPart>
#include <QJsonArray>
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

    Station station{title.isEmpty() ? title
                                    : tr("Radio Record - %0").arg(title),
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
        &RadioRecord::onSearchRequestFinished);
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
    if (!reply.isSuccess()) {
        raiseError(tr("Search error"), reply.networkReply()->errorString());

        return;
    }

    QJsonDocument json;

    if (!parseJson(reply, &json)) {
        return;
    }

    const QString path = Utilities::requestPathFromRestReply(reply);

    if (path.endsWith(RadioRecordConstants::DefaultStationsPath)) {
        handleStationsEndpointResult(json);
    } else if (path.endsWith(RadioRecordConstants::SearchPath)) {
        handleSearchEndpointResult(json);
    } else {
        raiseError(tr("Search error"), tr("Unhandled path %0").arg(path));
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
