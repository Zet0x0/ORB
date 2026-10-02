#pragma once

#include <QAbstractListModel>
#include <QByteArray>
#include <QHash>
#include <QList>
#include <QQmlEngine>
#include <QString>
#include <QVariant>

class SettingsCategoryModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

private:
    struct Category {
        QByteArray id;
        QString name;
    };

    QList<Category> m_categories;

    void rebuildCategories();

public:
    enum CategoryRoles { NameRole = Qt::UserRole, IdRole };
    Q_ENUM(CategoryRoles)

    explicit SettingsCategoryModel(QObject *parent = nullptr);

    Q_INVOKABLE int
    rowCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_INVOKABLE QVariant data(const QModelIndex &index,
                              int role) const override;

    QHash<int, QByteArray> roleNames() const override;
};
