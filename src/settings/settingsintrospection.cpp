#include "settingsintrospection.h"

namespace SettingsIntrospection {
QString label(const QByteArray &propertyName) {
    QString result;

    for (int i = 0; i < propertyName.size(); ++i) {
        const QChar ch = QLatin1Char(propertyName.at(i));

        if (i == 0) {
            result += ch.toUpper();

            continue;
        }

        if (ch.isUpper()) {
            result += QLatin1Char(' ');
            result += ch.toLower();

            continue;
        }

        result += ch;
    }

    return result;
}

QList<ResolvedField> resolvedFields(const SettingsGroup *group) {
    QList<ResolvedField> result;

    if (!group) {
        return result;
    }

    const QMetaObject *metaObject = group->metaObject();
    const QList<SettingsFieldMeta> fields = group->settingsFields();

    for (const SettingsFieldMeta &field : std::as_const(fields)) {
        const int propertyIndex =
            metaObject->indexOfProperty(field.propertyName.constData());

        if (propertyIndex == -1) {
            continue;
        }

        const QMetaProperty property = metaObject->property(propertyIndex);

        if (!property.isWritable() || !property.hasNotifySignal()) {
            continue;
        }

        result.append(
            {property,
             field.label.isEmpty() ? label(field.propertyName) : field.label,
             field.subcategory.isEmpty() ? group->settingsSubcategory()
                                         : field.subcategory});
    }

    return result;
}
}
