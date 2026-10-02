#include "logfiltermodel.h"
#include "logger.h"
#include <QDateTime>
#include <QStringList>
#include <bit>

bool LogFilterModel::showsLevel(int level) const {
    return (m_levelMask & (1 << level)) != 0;
}

LogFilterModel::LogFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent) {
    setSourceModel(Logger::instance());

    connect(this, &QAbstractItemModel::rowsInserted, this,
            &LogFilterModel::countChanged);
    connect(this, &QAbstractItemModel::rowsRemoved, this,
            &LogFilterModel::countChanged);
    connect(this, &QAbstractItemModel::modelReset, this,
            &LogFilterModel::countChanged);
}

void LogFilterModel::setShowsLevel(int level, bool shown) {
    if (showsLevel(level) == shown) {
        return;
    }

    beginFilterChange();
    m_levelMask ^= 1 << level;
    endFilterChange(QSortFilterProxyModel::Direction::Rows);

    emit hiddenLevelCountChanged();
}

int LogFilterModel::hiddenLevelCount() const {
    return std::popcount(~static_cast<unsigned int>(m_levelMask));
}

QString LogFilterModel::query() const {
    return m_query;
}

void LogFilterModel::setQuery(const QString &query) {
    if (m_query == query) {
        return;
    }

    beginFilterChange();
    m_query = query;
    endFilterChange(QSortFilterProxyModel::Direction::Rows);

    emit queryChanged();
}

int LogFilterModel::count() const {
    return rowCount();
}

int LogFilterModel::sortedColumn() const {
    return sortColumn();
}

Qt::SortOrder LogFilterModel::sortedOrder() const {
    return sortOrder();
}

void LogFilterModel::cycleSort(int column) {
    if (column != sortColumn()) {
        sort(column, Qt::AscendingOrder);
    } else if (sortOrder() == Qt::AscendingOrder) {
        sort(column, Qt::DescendingOrder);
    } else {
        sort(-1);
    }

    emit sortChanged();
}

QString LogFilterModel::formattedText() const {
    QStringList lines;

    lines.reserve(rowCount());

    for (int row = 0; row < rowCount(); ++row) {
        lines << index(row, 0).data(Logger::LineTextRole).toString();
    }

    return lines.join(u'\n');
}

bool LogFilterModel::filterAcceptsRow(int sourceRow,
                                      const QModelIndex &sourceParent) const {
    const QAbstractItemModel *source = sourceModel();

    const int level =
        source->index(sourceRow, Logger::LevelColumn, sourceParent)
            .data(Logger::LevelRole)
            .toInt();

    if (!showsLevel(level)) {
        return false;
    }

    if (m_query.isEmpty()) {
        return true;
    }

    return source->index(sourceRow, Logger::MessageColumn, sourceParent)
               .data()
               .toString()
               .contains(m_query, Qt::CaseInsensitive) ||
           source->index(sourceRow, Logger::CategoryColumn, sourceParent)
               .data()
               .toString()
               .contains(m_query, Qt::CaseInsensitive);
}

bool LogFilterModel::lessThan(const QModelIndex &left,
                              const QModelIndex &right) const {
    switch (left.column()) {
    case Logger::TimeColumn:
        return left.data(Logger::TimestampRole).toDateTime() <
               right.data(Logger::TimestampRole).toDateTime();

    case Logger::LevelColumn:
        return left.data(Logger::LevelRole).toInt() <
               right.data(Logger::LevelRole).toInt();

    default:
        return QString::compare(left.data(Qt::DisplayRole).toString(),
                                right.data(Qt::DisplayRole).toString(),
                                Qt::CaseInsensitive) < 0;
    }
}
