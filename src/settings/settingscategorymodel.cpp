#include "settingscategorymodel.h"
#include "settingsgroup.h"
#include "settingsintrospection.h"
#include <QCollator>

void SettingsCategoryModel::rebuildCategories() {
    m_categories.clear();

    const QList<SettingsGroup *> groups = SettingsIntrospection::groups();

    // Only show categories that actually have fields in them
    for (const SettingsGroup *group : groups) {
        const QList<SettingsIntrospection::ResolvedField> fields =
            SettingsIntrospection::resolvedFields(group);

        for (const SettingsIntrospection::ResolvedField &field :
             std::as_const(fields)) {
            const bool exists =
                std::any_of(m_categories.cbegin(), m_categories.cend(),
                            [&](const Category &category) {
                                return category.id == field.categoryId;
                            });

            if (!exists) {
                m_categories.append(
                    {.id = field.categoryId,
                     .name = SettingsIntrospection::categoryName(
                         field.categoryId)});
            }
        }
    }

    QCollator collator;

    std::sort(m_categories.begin(), m_categories.end(),
              [&collator](const Category &a, const Category &b) {
                  return collator.compare(a.name, b.name) < 0;
              });
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
