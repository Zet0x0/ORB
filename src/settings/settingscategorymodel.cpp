#include "settingscategorymodel.h"
#include "settings.h"
#include "settingsgroup.h"
#include "settingsintrospection.h"
#include <QCollator>

SettingsCategoryModel::SettingsCategoryModel(QObject *parent)
    : QAbstractListModel(parent) {
    rebuildCategories();
}

void SettingsCategoryModel::rebuildCategories() {
    m_categories.clear();

    QObject *root = Settings::instance();
    const QMetaObject *metaObject = root->metaObject();

    for (int i = QObject::staticMetaObject.propertyCount();
         i < metaObject->propertyCount(); ++i) {
        const QMetaProperty property = metaObject->property(i);

        if (!property.metaType().flags().testFlag(
                QMetaType::PointerToQObject)) {
            continue;
        }

        auto *group = qobject_cast<SettingsGroup *>(
            property.read(root).value<QObject *>());

        if (!group || SettingsIntrospection::resolvedFields(group).isEmpty()) {
            continue;
        }

        const QString categoryName = group->settingsCategory();

        auto existingCategory =
            std::find_if(m_categories.begin(), m_categories.end(),
                         [&](const Category &category) {
                             return category.name == categoryName;
                         });

        if (existingCategory == m_categories.end()) {
            m_categories.append({categoryName, {group}});
        } else {
            existingCategory->groups.append(group);
        }
    }

    QCollator collator;

    std::sort(m_categories.begin(), m_categories.end(),
              [&collator](const Category &a, const Category &b) {
                  return collator.compare(a.name, b.name) < 0;
              });
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

    case GroupsRole: {
        QVariantList groups;

        for (QObject *group : category.groups) {
            groups.append(QVariant::fromValue(group));
        }

        return groups;
    }

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> SettingsCategoryModel::roleNames() const {
    static const QHash<int, QByteArray> roles{
        {NameRole, QByteArrayLiteral("name")},
        {GroupsRole, QByteArrayLiteral("groups")}};

    return roles;
}
