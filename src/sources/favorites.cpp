#include "favorites.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
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
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Favorites: cannot read" << m_filePath
                   << file.errorString();

        return;
    }

    QJsonParseError error;
    const QJsonDocument document =
        QJsonDocument::fromJson(file.readAll(), &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Favorites: ignoring unparseable" << m_filePath
                   << error.errorString();

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
}

void Favorites::persist() const {
    QJsonArray array;

    for (const Station &station : m_stations) {
        array.append(QJsonObject::fromVariantMap(station.toMap()));
    }

    QDir().mkpath(QFileInfo(m_filePath).absolutePath());

    QSaveFile file(m_filePath);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Favorites: cannot write" << m_filePath
                   << file.errorString();

        return;
    }

    file.write(QJsonDocument(array).toJson(QJsonDocument::Indented));

    if (!file.commit()) {
        qWarning() << "Favorites: failed to commit" << m_filePath
                   << file.errorString();
    }
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

    persist();

    emit changed();
}

void Favorites::remove(const Station &station) {
    const int index = indexOf(station);

    if (index == -1) {
        return;
    }

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
