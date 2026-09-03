#pragma once

#include "../common/singleton.h"
#include "station.h"
#include <QQmlEngine>

class Favorites : public QObject, public Singleton<Favorites> {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(int count READ count NOTIFY changed FINAL)

    friend class Singleton<Favorites>;

private:
    QString m_filePath;
    QList<Station> m_stations;

    explicit Favorites(QObject *parent = nullptr);

    int indexOf(const Station &station) const;

    void load();
    void persist() const;

public:
    int count() const;

    QList<Station> stations() const;

    Q_INVOKABLE bool contains(const Station &station) const;
    Q_INVOKABLE void add(const Station &station);
    Q_INVOKABLE void remove(const Station &station);
    Q_INVOKABLE void toggle(const Station &station);

signals:
    void changed();
};
