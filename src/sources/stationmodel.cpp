#include "stationmodel.h"

StationModel::StationModel(QObject *parent) : QAbstractListModel(parent) {}

int StationModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)

    return m_stations.size();
}

QVariant StationModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case StationRoles::StationRole:
        return QVariant::fromValue(m_stations[index.row()]);

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
        {StationRoles::StationRole, QByteArrayLiteral("station")}};

    return roles;
}

void StationModel::setStations(const QList<Station> &stations) {
    beginResetModel();

    m_stations = stations;

    endResetModel();
}

void StationModel::clear() {
    beginResetModel();

    m_stations.clear();

    endResetModel();
}
