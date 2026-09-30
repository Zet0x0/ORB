#include "settingscategorymodel.h"
#include "settingscategories.h"
#include "settingsgroup.h"
#include "settingsintrospection.h"

void SettingsCategoryModel::rebuildCategories() {
    m_categories.clear();

    const QList<SettingsGroup *> groups = SettingsIntrospection::groups();

    QSet<QByteArray> usedIds;

    for (const SettingsGroup *group : groups) {
        const QList<SettingsIntrospection::ResolvedField> fields =
            SettingsIntrospection::resolvedFields(group);

        for (const SettingsIntrospection::ResolvedField &field :
             std::as_const(fields)) {
            usedIds.insert(field.categoryId);
        }
    }

    // only show categories that have UI-shown fields in them,
    // in the order from settingscategories.h
    for (const char *id : SettingsCategory::Order) {
        if (usedIds.contains(id)) {
            m_categories.append(
                {.id = id, .name = SettingsIntrospection::categoryName(id)});
        }
    }
}

SettingsCategoryModel::SettingsCategoryModel(QObject *parent)
    : QAbstractListModel(parent) {
    rebuildCategories();
}

int SettingsCategoryModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : m_categories.size();
}

QVariant SettingsCategoryModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_categories.size()) {
        return QVariant();
    }

    const Category &category = m_categories.at(index.row());

    switch (role) {
    case NameRole:
        return category.name;

    case IdRole:
        return QString::fromUtf8(category.id);

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> SettingsCategoryModel::roleNames() const {
    static const QHash<int, QByteArray> roles{
        {NameRole, QByteArrayLiteral("name")},
        {IdRole, QByteArrayLiteral("categoryId")}};

    return roles;
}
