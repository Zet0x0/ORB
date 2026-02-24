#include "radiorecord.h"
#include "../utilities.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QRestAccessManager>
#include <QRestReply>

bool RadioRecord::parseJson(QRestReply &reply, QJsonDocument *json) {
    QJsonParseError jsonError;
    const std::optional jsonDocument = reply.readJson(&jsonError);

    if (jsonError.error != QJsonParseError::NoError) {
        raiseError(tr("Parse Error"),
                   tr("%0 (offset %1)")
                       .arg(jsonError.errorString(),
                            QString::number(jsonError.offset)));

        return false;
    }

    *json = *jsonDocument;

    return true;
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

        Station station{
            rawStationObject.value(QStringLiteral("title")).toString(),
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

    m_defaultStations.setCached(stations);

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

    if (path.endsWith(Api::Paths::Stations)) {
        handleStationsEndpointResult(json);
    } else {
        raiseError(tr("Search Error"), tr("Unhandled path %0").arg(path));
    }
}

RadioRecord::RadioRecord()
    : m_networkAccessManager(new QNetworkAccessManager(this)),
      m_restAccessManager(
          new QRestAccessManager(m_networkAccessManager, this)) {}

// TODO
void RadioRecord::search(const QString &query) {}

// TODO
void RadioRecord::cancelSearch() {}

bool RadioRecord::hasDefaultStations() const {
    return true;
}

void RadioRecord::loadDefaultStations() {
    emit searchStarted();

    const QList<Station> cached = m_defaultStations.getCached();

    if (!cached.isEmpty()) {
        emit stationsDispatched(cached);

        return;
    }

    m_restAccessManager->get(m_api.createRequest(Api::Paths::Stations), this,
                             &RadioRecord::onSearchRequestFinished);
}

void RadioRecord::DefaultStations::resetCache() {
    m_stations.clear();
    m_cachedAt = -1;
}

void RadioRecord::DefaultStations::setCached(const QList<Station> &stations) {
    if (stations.isEmpty()) {
        resetCache();

        return;
    }

    m_stations = stations;
    m_cachedAt = Utilities::currentTimestampUtc();
}

const QList<Station> &RadioRecord::DefaultStations::getCached() {
    if (Utilities::currentTimestampUtc() - m_cachedAt >
        Api::CacheExpiries::Stations) {
        resetCache();
    }

    return m_stations;
}
