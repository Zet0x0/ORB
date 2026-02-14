#include "stationmodel.h"

int StationModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)

    return m_stations.size();
}

QVariant StationModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case StationModel::StationRoles::IsValidRole:
        return m_stations[index.row()].isValid();
    case StationModel::StationRoles::NameRole:
        return m_stations[index.row()].name();
    case StationModel::StationRoles::StreamUrlRole:
        return m_stations[index.row()].streamUrl();
    case StationModel::StationRoles::ImageUrlRole:
        return m_stations[index.row()].imageUrl();

    default:
        return QVariant();
    }
}

Qt::ItemFlags StationModel::flags(const QModelIndex &index) const {
    Q_UNUSED(index)

    return Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
}

QHash<int, QByteArray> StationModel::roleNames() const {
    static const QHash<int, QByteArray> roles{
        {StationModel::StationRoles::IsValidRole, QByteArrayLiteral("valid")},
        {StationModel::StationRoles::NameRole, QByteArrayLiteral("name")},
        {StationModel::StationRoles::StreamUrlRole,
         QByteArrayLiteral("streamUrl")},
        {StationModel::StationRoles::ImageUrlRole,
         QByteArrayLiteral("imageUrl")}};

    return roles;
}

void StationModel::setStations(const QList<Station> &stations) {
    beginInsertRows(QModelIndex(), 0, stations.size() - 1);

    m_stations = stations;

    endInsertRows();
}

void StationModel::clear() {
    beginResetModel();

    m_stations.clear();

    endResetModel();
}
