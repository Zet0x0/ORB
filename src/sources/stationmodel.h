#pragma once

#include "../player/station.h"
#include <QAbstractListModel>
#include <QQmlEngine>

class StationModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

private:
    QList<Station> m_stations;

public:
    enum StationRoles { StationRole = Qt::UserRole };
    Q_ENUM(StationRoles)

    Q_INVOKABLE int
    rowCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_INVOKABLE QVariant
    data(const QModelIndex &index,
         int role = StationRoles::StationRole) const override;

    Q_INVOKABLE Qt::ItemFlags flags(const QModelIndex &index) const override;

    QHash<int, QByteArray> roleNames() const override;

    void setStations(const QList<Station> &stations);
    void clear();
};
