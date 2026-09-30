#include "settingsintrospection.h"
#include "settings.h"
#include <QCoreApplication>

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

QString categoryName(const QByteArray &id) {
    if (id.isEmpty()) {
        return QString();
    }

    return QCoreApplication::translate("SettingsCategory", id.constData());
}

QList<SettingsGroup *> groups() {
    QList<SettingsGroup *> result;

    Settings *root = Settings::instance();
    const QMetaObject *metaObject = root->metaObject();

    for (int i = QObject::staticMetaObject.propertyCount();
         i < metaObject->propertyCount(); ++i) {
        const QMetaProperty property = metaObject->property(i);

        if (!property.metaType().flags().testFlag(
                QMetaType::PointerToQObject)) {
            continue;
        }

        if (SettingsGroup *group = qobject_cast<SettingsGroup *>(
                property.read(root).value<QObject *>())) {
            result.append(group);
        }
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
            qWarning() << "SettingsIntrospection:" << metaObject->className()
                       << "has no property" << field.propertyName;

            continue;
        }

        const QMetaProperty property = metaObject->property(propertyIndex);

        if (!property.isWritable() || !property.hasNotifySignal()) {
            qWarning() << "SettingsIntrospection:" << metaObject->className()
                       << "property" << field.propertyName
                       << "needs to be writable and have a NOTIFY";

            continue;
        }

        if (field.category.isEmpty()) {
            qWarning() << "SettingsIntrospection:" << metaObject->className()
                       << "property" << field.propertyName << "has no category";

            continue;
        }

        result.append({.property = property,
                       .label = field.label.isEmpty()
                                  ? label(field.propertyName)
                                  : field.label,
                       .categoryId = field.category,
                       .subcategoryId = field.subcategory,
                       .subcategory = categoryName(field.subcategory),
                       .min = field.min,
                       .max = field.max,
                       .step = field.step});
    }

    return result;
}
}
