#include "settingspropertymodel.h"
#include "settingsgroup.h"
#include "settingsintrospection.h"

SettingsPropertyModel::SettingsPropertyModel(QObject *parent)
    : QAbstractListModel(parent) {}

QVariant SettingsPropertyModel::groups() const {
    QVariantList result;

    for (QObject *group : m_groups) {
        result.append(QVariant::fromValue(group));
    }

    return result;
}

void SettingsPropertyModel::setGroups(const QVariant &newGroups) {
    QList<QObject *> groups;

    const QVariantList newGroupsList = newGroups.toList();

    for (const QVariant &value : std::as_const(newGroupsList)) {
        if (QObject *group = value.value<QObject *>()) {
            groups.append(group);
        }
    }

    if (m_groups == groups) {
        return;
    }

    beginResetModel();

    m_groups = groups;
    rebuildEntries();

    endResetModel();

    emit groupsChanged();
}

void SettingsPropertyModel::rebuildEntries() {
    m_entries.clear();

    for (QObject *groupObject : std::as_const(m_groups)) {
        auto *group = qobject_cast<SettingsGroup *>(groupObject);

        const QList<SettingsIntrospection::ResolvedField> fields =
            SettingsIntrospection::resolvedFields(group);

        for (const SettingsIntrospection::ResolvedField &field :
             std::as_const(fields)) {
            m_entries.append({groupObject, field.property, field.label,
                              field.subcategory, field.min, field.max});
        }
    }

    std::stable_sort(
        m_entries.begin(), m_entries.end(), [](const Entry &a, const Entry &b) {
            return a.subcategory.isEmpty() && !b.subcategory.isEmpty();
        });
}

int SettingsPropertyModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : m_entries.size();
}

QVariant SettingsPropertyModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_entries.size()) {
        return QVariant();
    }

    const Entry &entry = m_entries.at(index.row());

    switch (role) {
    case NameRole:
        return QString::fromUtf8(entry.property.name());

    case LabelRole:
        return entry.label;

    case TypeRole:
        return propertyType(entry.property);

    case ValueRole: {
        const int pendingIndex =
            pendingChangeIndex(entry.target, entry.property);

        if (pendingIndex != -1) {
            return m_pendingChanges.at(pendingIndex).value;
        }

        return entry.property.read(entry.target);
    }

    case SubcategoryRole:
        return entry.subcategory;

    case MinRole:
        return entry.min;

    case MaxRole:
        return entry.max;

    default:
        return QVariant();
    }
}

bool SettingsPropertyModel::setData(const QModelIndex &index,
                                    const QVariant &value, int role) {
    if (role != ValueRole || !index.isValid() ||
        index.row() >= m_entries.size()) {
        return false;
    }

    const Entry &entry = m_entries.at(index.row());
    const int pendingIndex = pendingChangeIndex(entry.target, entry.property);

    if (value == entry.property.read(entry.target)) {
        if (pendingIndex != -1) {
            m_pendingChanges.removeAt(pendingIndex);
        }
    } else if (pendingIndex != -1) {
        m_pendingChanges[pendingIndex].value = value;
    } else {
        m_pendingChanges.append({entry.target, entry.property, value});
    }

    emit dataChanged(index, index, {ValueRole});
    emit hasPendingChangesChanged();

    return true;
}

void SettingsPropertyModel::setValue(int row, const QVariant &value) {
    setData(index(row), value, ValueRole);
}

bool SettingsPropertyModel::hasPendingChanges() const {
    return !m_pendingChanges.isEmpty();
}

int
SettingsPropertyModel::pendingChangeIndex(QObject *target,
                                          const QMetaProperty &property) const {
    for (int i = 0; i < m_pendingChanges.size(); ++i) {
        const PendingChange &change = m_pendingChanges.at(i);

        if (change.target == target &&
            change.property.propertyIndex() == property.propertyIndex()) {
            return i;
        }
    }

    return -1;
}

void SettingsPropertyModel::applyChanges() {
    if (m_pendingChanges.isEmpty()) {
        return;
    }

    for (const PendingChange &change : std::as_const(m_pendingChanges)) {
        change.property.write(change.target, change.value);
    }

    clearPendingChanges();
}

void SettingsPropertyModel::discardChanges() {
    clearPendingChanges();
}

void SettingsPropertyModel::clearPendingChanges() {
    if (m_pendingChanges.isEmpty()) {
        return;
    }

    m_pendingChanges.clear();

    if (!m_entries.isEmpty()) {
        emit dataChanged(index(0), index(m_entries.size() - 1), {ValueRole});
    }

    emit hasPendingChangesChanged();
}

QHash<int, QByteArray> SettingsPropertyModel::roleNames() const {
    static const QHash<int, QByteArray> roles{
        {NameRole, QByteArrayLiteral("name")},
        {LabelRole, QByteArrayLiteral("label")},
        {TypeRole, QByteArrayLiteral("type")},
        {ValueRole, QByteArrayLiteral("value")},
        {SubcategoryRole, QByteArrayLiteral("subcategory")},
        {MinRole, QByteArrayLiteral("min")},
        {MaxRole, QByteArrayLiteral("max")}};

    return roles;
}

QString SettingsPropertyModel::propertyType(const QMetaProperty &property) {
    switch (property.metaType().id()) {
    case QMetaType::Bool:
        return QStringLiteral("bool");

    case QMetaType::Int:
        return QStringLiteral("int");

    default:
        return QStringLiteral("string");
    }
}
