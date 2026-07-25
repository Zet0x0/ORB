#pragma once

#include <QAbstractListModel>
#include <QMetaProperty>
#include <QQmlEngine>

class SettingsPropertyModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(
        QVariant groups READ groups WRITE setGroups NOTIFY groupsChanged FINAL)

private:
    struct Entry {
        QObject *target;
        QMetaProperty property;
        QString label;
        QString subcategory;
    };

    QList<QObject *> m_groups;
    QList<Entry> m_entries;

    void rebuildEntries();

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

signals:
    void groupsChanged();
};
