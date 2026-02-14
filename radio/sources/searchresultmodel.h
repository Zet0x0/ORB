#pragma once

#include "../player/station.h"
#include <QAbstractListModel>
#include <QQmlEngine>

class SearchResultModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

private:
    QList<Station> m_stations;

public:
    enum StationRoles {
        IsValidRole = Qt::UserRole,
        NameRole,
        StreamUrlRole,
        ImageUrlRole
    };
    Q_ENUM(SearchResultModel::StationRoles)

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant
    data(const QModelIndex &index,
         int role = SearchResultModel::StationRoles::NameRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QHash<int, QByteArray> roleNames() const override;

    void setStations(const QList<Station> &stations);
    void clear();
};
