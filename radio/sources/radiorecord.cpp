#include "radiorecord.h"
#include <QJsonArray>
#include <QJsonObject>

void RadioRecord::handleLoadDefaultStations() {
    m_restAccessManager->get(
        m_api.createRequest(RadioRecordConstants::StationsPath), this,
        &RadioRecord::onSearchRequestFinished);
}

void RadioRecord::handleStationsEndpointResult(const QJsonDocument &json) {
    const QJsonArray rawStations = json.object()
                                       .value(QStringLiteral("result"))
                                       .toObject()
                                       .value(QStringLiteral("stations"))
                                       .toArray();
    QList<Station> stations;

    for (const QJsonValue &rawStation : rawStations) {
        const QJsonObject rawStationObject = rawStation.toObject();

        if (rawStationObject.isEmpty()) {
            continue;
        }

        const QString title =
            rawStationObject.value(QStringLiteral("title")).toString();

        Station station{
            title.isEmpty() ? title : tr("Radio Record - %0").arg(title),
            rawStationObject.value(QStringLiteral("stream_hls")).toString(),
            rawStationObject.value(QStringLiteral("icon_gray")).toString()};

        if (station.isValid()) {
            stations << station;
        }
    }

    if (stations.isEmpty()) {
        raiseError(tr("Search Error"), tr("No default stations found"));

        return;
    }

    cacheDefaultStations(stations);

    emit stationsDispatched(stations);
}

void RadioRecord::onSearchRequestFinished(QRestReply &reply) {
    if (!reply.isSuccess()) {
        raiseError(tr("Search Error"), reply.errorString());

        return;
    }

    QJsonDocument json;

    if (!parseJson(reply, &json)) {
        return;
    }

    const QString path = reply.networkReply()->url().path();

    if (path.endsWith(RadioRecordConstants::StationsPath)) {
        handleStationsEndpointResult(json);
    } else {
        raiseError(tr("Search Error"), tr("Unhandled path %0").arg(path));
    }
}

// TODO
void RadioRecord::search(const QString &query) {}

// TODO
void RadioRecord::cancelSearch() {}

bool RadioRecord::hasDefaultStations() const {
    return true;
}
