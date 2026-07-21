#include "source.h"
#include "../common/utilities.h"
#include <QJsonDocument>
#include <QJsonParseError>

void Source::raiseError(const QString &title, const QString &message) {
    SourceError error;

    error.title = title;
    error.message = message;

    emit errorOccurred(error);
}

bool Source::parseJson(QRestReply &reply, QJsonDocument *json) {
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

void Source::cacheDefaultStations(const QList<Station> &stations) {
    m_defaultStations.setCached(stations);
}

const QList<Station> &Source::getCachedDefaultStations() {
    return m_defaultStations.getCached();
}

void Source::handleLoadDefaultStations() {}

Source::Source()
    : m_networkAccessManager(new QNetworkAccessManager(this)),
      m_restAccessManager(
          new QRestAccessManager(m_networkAccessManager, this)) {}

void Source::search(const QString &query) {
    emit searchStarted();

    handleSearch(query);
}

bool Source::hasDefaultStations() const {
    return false;
}

void Source::loadDefaultStations() {
    if (!hasDefaultStations()) {
        return;
    }

    emit searchStarted();

    const QList<Station> cached = getCachedDefaultStations();

    if (!cached.isEmpty()) {
        emit stationsDispatched(cached);

        return;
    }

    handleLoadDefaultStations();
}

void Source::DefaultStations::resetCache() {
    m_stations.clear();
    m_cachedAt = -1;
}

void Source::DefaultStations::setCached(const QList<Station> &stations) {
    if (stations.isEmpty()) {
        resetCache();

        return;
    }

    m_stations = stations;
    m_cachedAt = Utilities::currentTimestampUtc();
}

const QList<Station> &Source::DefaultStations::getCached() {
    if (Utilities::currentTimestampUtc() - m_cachedAt >
        SourceConstraints::DefaultStationsCacheExpiry) {
        resetCache();
    }

    return m_stations;
}
