#include "settingspropertymodel.h"
#include "settingsgroup.h"
#include "settingsintrospection.h"

void SettingsPropertyModel::rebuildEntries() {
    m_entries.clear();

    if (m_categoryId.isEmpty()) {
        return;
    }

    const QList<SettingsGroup *> groups = SettingsIntrospection::groups();

    for (SettingsGroup *group : groups) {
        const QList<SettingsIntrospection::ResolvedField> fields =
            SettingsIntrospection::resolvedFields(group);

        for (const SettingsIntrospection::ResolvedField &field :
             std::as_const(fields)) {
            if (field.categoryId != m_categoryId) {
                continue;
            }

            m_entries.append({.target = group,
                              .property = field.property,
                              .label = field.label,
                              .subcategoryId = field.subcategoryId,
                              .subcategory = field.subcategory,
                              .min = field.min,
                              .max = field.max,
                              .step = field.step});
        }
    }

    // Fields without a subcategory go first, then go the subcategories in
    // declared order
    QList<QByteArray> subcategoryOrder{QByteArray()};

    for (const Entry &entry : std::as_const(m_entries)) {
        if (!subcategoryOrder.contains(entry.subcategoryId)) {
            subcategoryOrder.append(entry.subcategoryId);
        }
    }

    std::stable_sort(m_entries.begin(), m_entries.end(),
                     [&subcategoryOrder](const Entry &a, const Entry &b) {
                         return subcategoryOrder.indexOf(a.subcategoryId) <
                                subcategoryOrder.indexOf(b.subcategoryId);
                     });
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

SettingsPropertyModel::SettingsPropertyModel(QObject *parent)
    : QAbstractListModel(parent) {}

QString SettingsPropertyModel::categoryId() const {
    return QString::fromUtf8(m_categoryId);
}

void SettingsPropertyModel::setCategoryId(const QString &newCategoryId) {
    const QByteArray categoryId = newCategoryId.toUtf8();

    if (m_categoryId == categoryId) {
        return;
    }

    beginResetModel();

    m_categoryId = categoryId;
    rebuildEntries();

    endResetModel();

    emit categoryIdChanged();
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

    case SubcategoryStartRole:
        return !entry.subcategoryId.isEmpty() &&
               (index.row() == 0 ||
                m_entries.at(index.row() - 1).subcategoryId !=
                    entry.subcategoryId);

    case MinRole:
        return entry.min;

    case MaxRole:
        return entry.max;

    case StepRole:
        return entry.step;

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
        m_pendingChanges.append({.target = entry.target,
                                 .property = entry.property,
                                 .value = value});
    }

    emit dataChanged(index, index, {ValueRole});
    emit hasPendingChangesChanged();

    return true;
}

QHash<int, QByteArray> SettingsPropertyModel::roleNames() const {
    static const QHash<int, QByteArray> roles{
        {NameRole, QByteArrayLiteral("name")},
        {LabelRole, QByteArrayLiteral("label")},
        {TypeRole, QByteArrayLiteral("type")},
        {ValueRole, QByteArrayLiteral("value")},
        {SubcategoryRole, QByteArrayLiteral("subcategory")},
        {SubcategoryStartRole, QByteArrayLiteral("subcategoryStart")},
        {MinRole, QByteArrayLiteral("min")},
        {MaxRole, QByteArrayLiteral("max")},
        {StepRole, QByteArrayLiteral("step")}};

    return roles;
}

void SettingsPropertyModel::setValue(int row, const QVariant &value) {
    setData(index(row), value, ValueRole);
}

bool SettingsPropertyModel::hasPendingChanges() const {
    return !m_pendingChanges.isEmpty();
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
