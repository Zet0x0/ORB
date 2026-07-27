#pragma once

#include <QAbstractListModel>
#include <QMetaProperty>
#include <QQmlEngine>

class SettingsPropertyModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(
        QVariant groups READ groups WRITE setGroups NOTIFY groupsChanged FINAL)
    Q_PROPERTY(bool hasPendingChanges READ hasPendingChanges NOTIFY
                   hasPendingChangesChanged FINAL)

private:
    struct Entry {
        QObject *target;
        QMetaProperty property;
        QString label;
        QString subcategory;
    };

    struct PendingChange {
        QObject *target;
        QMetaProperty property;
        QVariant value;
    };

    QList<QObject *> m_groups;
    QList<Entry> m_entries;
    QList<PendingChange> m_pendingChanges;

    void rebuildEntries();
    void clearPendingChanges();
    int pendingChangeIndex(QObject *target,
                           const QMetaProperty &property) const;

    static QString propertyType(const QMetaProperty &property);

public:
    enum PropertyRoles {
        NameRole = Qt::UserRole,
        LabelRole,
        TypeRole,
        ValueRole,
        SubcategoryRole
    };
    Q_ENUM(PropertyRoles)

    explicit SettingsPropertyModel(QObject *parent = nullptr);

    QVariant groups() const;
    void setGroups(const QVariant &newGroups);

    Q_INVOKABLE int
    rowCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_INVOKABLE QVariant data(const QModelIndex &index,
                              int role) const override;
    Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value,
                             int role = ValueRole) override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void setValue(int row, const QVariant &value);

    bool hasPendingChanges() const;

    Q_INVOKABLE void applyChanges();
    Q_INVOKABLE void discardChanges();

signals:
    void groupsChanged();
    void hasPendingChangesChanged();
};
