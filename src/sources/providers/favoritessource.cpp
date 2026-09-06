#include "favoritessource.h"
#include "../favorites.h"

FavoritesSource::FavoritesSource() {
    connect(Favorites::instance(), &Favorites::changed, this,
            &FavoritesSource::dispatch);
}

QList<Station> FavoritesSource::filtered() const {
    const QList<Station> all = Favorites::instance()->stations();

    if (m_query.isEmpty()) {
        return all;
    }

    QList<Station> result;

    for (const Station &station : all) {
        if (station.name().contains(m_query, Qt::CaseInsensitive)) {
            result.append(station);
        }
    }

    return result;
}

void FavoritesSource::dispatch() {
    const QList<Station> stations = filtered();

    if (stations.isEmpty() && m_query.isEmpty()) {
        raiseError(tr("No favorites yet"),
                   tr("Favorite a station to add it here"));

        return;
    }

    emit stationsDispatched(stations);
}

void FavoritesSource::handleSearch(const QString &query) {
    m_query = query.trimmed();

    dispatch();
}

void FavoritesSource::handleLoadDefaultStations() {
    m_query.clear();

    dispatch();
}

void FavoritesSource::cancelSearch() {}

bool FavoritesSource::hasDefaultStations() const {
    return true;
}

QString FavoritesSource::websiteUrl() const {
    return QString();
}
