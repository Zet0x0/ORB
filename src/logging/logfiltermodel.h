#pragma once

#include <QQmlEngine>
#include <QSortFilterProxyModel>

class LogFilterModel : public QSortFilterProxyModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(
        QString query READ query WRITE setQuery NOTIFY queryChanged FINAL)
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)
    Q_PROPERTY(int hiddenLevelCount READ hiddenLevelCount NOTIFY
                   hiddenLevelCountChanged FINAL)

    Q_PROPERTY(int sortedColumn READ sortedColumn NOTIFY sortChanged FINAL)
    Q_PROPERTY(
        Qt::SortOrder sortedOrder READ sortedOrder NOTIFY sortChanged FINAL)

private:
    int m_levelMask = ~0;

    QString m_query;

    bool showsLevel(int level) const;

public:
    explicit LogFilterModel(QObject *parent = nullptr);

    Q_INVOKABLE void setShowsLevel(int level, bool shown);
    int hiddenLevelCount() const;

    QString query() const;
    void setQuery(const QString &query);

    int count() const;

    int sortedColumn() const;
    Qt::SortOrder sortedOrder() const;

    Q_INVOKABLE void cycleSort(int column);

    Q_INVOKABLE QString formattedText() const;

signals:
    void queryChanged();
    void countChanged();
    void hiddenLevelCountChanged();
    void sortChanged();

protected:
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &left,
                  const QModelIndex &right) const override;
};
