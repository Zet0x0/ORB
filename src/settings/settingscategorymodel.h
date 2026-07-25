#pragma once

#include <QAbstractListModel>
#include <QQmlEngine>

class SettingsCategoryModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

private:
    struct Category {
        QString name;
        QList<QObject *> groups;
    };

    QList<Category> m_categories;

    void rebuildCategories();

public:
    enum CategoryRoles { NameRole = Qt::UserRole, GroupsRole };
    Q_ENUM(CategoryRoles)

    explicit SettingsCategoryModel(QObject *parent = nullptr);

    Q_INVOKABLE int
    rowCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_INVOKABLE QVariant data(const QModelIndex &index,
                              int role) const override;

    QHash<int, QByteArray> roleNames() const override;
};
