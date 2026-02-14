#include "searchresultmodel.h"

int SearchResultModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)

    return m_stations.size();
}

QVariant SearchResultModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case SearchResultModel::StationRoles::IsValidRole:
        return m_stations[index.row()].isValid();
    case SearchResultModel::StationRoles::NameRole:
        return m_stations[index.row()].name();
    case SearchResultModel::StationRoles::StreamUrlRole:
        return m_stations[index.row()].streamUrl();
    case SearchResultModel::StationRoles::ImageUrlRole:
        return m_stations[index.row()].imageUrl();

    default:
        return QVariant();
    }
}

Qt::ItemFlags SearchResultModel::flags(const QModelIndex &index) const {
    Q_UNUSED(index)

    return Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
}

QHash<int, QByteArray> SearchResultModel::roleNames() const {
    static const QHash<int, QByteArray> roles{
        {SearchResultModel::StationRoles::IsValidRole,
         QByteArrayLiteral("valid")},
        {SearchResultModel::StationRoles::NameRole, QByteArrayLiteral("name")},
        {SearchResultModel::StationRoles::StreamUrlRole,
         QByteArrayLiteral("streamUrl")},
        {SearchResultModel::StationRoles::ImageUrlRole,
         QByteArrayLiteral("imageUrl")}};

    return roles;
}

void SearchResultModel::setStations(const QList<Station> &stations) {
    beginInsertRows(QModelIndex(), 0, stations.size() - 1);

    m_stations = stations;

    endInsertRows();
}

void SearchResultModel::clear() {
    beginResetModel();

    m_stations.clear();

    endResetModel();
}
