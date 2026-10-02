#pragma once

#include <QAbstractListModel>
#include <QByteArray>
#include <QHash>
#include <QList>
#include <QMetaProperty>
#include <QQmlEngine>
#include <QString>
#include <QVariant>

class SettingsPropertyModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString categoryId READ categoryId WRITE setCategoryId NOTIFY
                   categoryIdChanged FINAL)
    Q_PROPERTY(bool hasPendingChanges READ hasPendingChanges NOTIFY
                   hasPendingChangesChanged FINAL)

private:
    struct Entry {
        QObject *target;
        QMetaProperty property;
        QString label;
        QString description;
        QByteArray subcategoryId;
        QString subcategory;

        int min;
        int max;
        int step;
    };

    struct PendingChange {
        QObject *target;
        QMetaProperty property;
        QVariant value;
    };

    QByteArray m_categoryId;
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
        DescriptionRole,
        TypeRole,
        ValueRole,
        SubcategoryRole,
        SubcategoryStartRole,
        MinRole,
        MaxRole,
        StepRole
    };
    Q_ENUM(PropertyRoles)

    explicit SettingsPropertyModel(QObject *parent = nullptr);

    QString categoryId() const;
    void setCategoryId(const QString &newCategoryId);

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
    void categoryIdChanged();
    void hasPendingChangesChanged();
};
