#include "favorites.h"
#include "../common/logcategories.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QSaveFile>
#include <QStandardPaths>

Favorites::Favorites(QObject *parent)
    : QObject(parent),
      m_filePath(QDir(QStandardPaths::writableLocation(
                          QStandardPaths::AppDataLocation))
                     .filePath(QStringLiteral("favorites.json"))) {
    load();
}

int Favorites::indexOf(const Station &station) const {
    for (int i = 0; i < m_stations.size(); ++i) {
        if (m_stations[i].streamUrl() == station.streamUrl()) {
            return i;
        }
    }

    return -1;
}

void Favorites::load() {
    QFile file(m_filePath);

    if (!file.exists()) {
        qCDebug(lcFavorites) << "No favorites file at" << m_filePath;

        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qCWarning(lcFavorites)
            << "Cannot read" << m_filePath << file.errorString();

        return;
    }

    QJsonParseError error;
    const QJsonDocument document =
        QJsonDocument::fromJson(file.readAll(), &error);

    if (error.error != QJsonParseError::NoError) {
        qCWarning(lcFavorites)
            << "Ignoring unparseable" << m_filePath << error.errorString();

        return;
    }

    if (!document.isArray()) {
        qCWarning(lcFavorites)
            << "Ignoring" << m_filePath << "as it doesn't hold a JSON array";

        return;
    }

    const QJsonArray array = document.array();

    for (const QJsonValue &value : array) {
        const Station station =
            Station::fromMap(value.toObject().toVariantMap());

        if (station.isValid() && indexOf(station) == -1) {
            m_stations.append(station);
        }
    }

    if (const qsizetype skipped = array.size() - m_stations.size();
        skipped > 0) {
        qCWarning(lcFavorites)
            << "Skipped" << skipped << "invalid or duplicate entries in"
            << m_filePath;
    }

    qCInfo(lcFavorites) << "Loaded" << m_stations.size() << "favorites from"
                        << m_filePath;
}

void Favorites::persist() const {
    QJsonArray array;

    for (const Station &station : m_stations) {
        array.append(QJsonObject::fromVariantMap(station.toMap()));
    }

    QDir().mkpath(QFileInfo(m_filePath).absolutePath());

    QSaveFile file(m_filePath);

    if (!file.open(QIODevice::WriteOnly)) {
        qCWarning(lcFavorites)
            << "Cannot write" << m_filePath << file.errorString();

        return;
    }

    file.write(QJsonDocument(array).toJson(QJsonDocument::Indented));

    if (!file.commit()) {
        qCWarning(lcFavorites)
            << "Failed to commit" << m_filePath << file.errorString();

        return;
    }

    qCDebug(lcFavorites) << "Saved" << m_stations.size() << "favorites to"
                         << m_filePath;
}

int Favorites::count() const {
    return m_stations.size();
}

QList<Station> Favorites::stations() const {
    return m_stations;
}

bool Favorites::contains(const Station &station) const {
    return indexOf(station) != -1;
}

void Favorites::add(const Station &station) {
    if (!station.isValid() || indexOf(station) != -1) {
        return;
    }

    m_stations.append(station);

    qCInfo(lcFavorites) << "Added" << station.name() << station.streamUrl();

    persist();

    emit changed();
}

void Favorites::remove(const Station &station) {
    const int index = indexOf(station);

    if (index == -1) {
        return;
    }

    qCInfo(lcFavorites) << "Removed" << m_stations.at(index).name()
                        << m_stations.at(index).streamUrl();

    m_stations.removeAt(index);

    persist();

    emit changed();
}

void Favorites::toggle(const Station &station) {
    if (contains(station)) {
        remove(station);
    } else {
        add(station);
    }
}
