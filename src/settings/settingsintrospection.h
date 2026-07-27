#pragma once

#include "settingsgroup.h"
#include <QMetaProperty>

namespace SettingsIntrospection {
struct ResolvedField {
    QMetaProperty property;
    QString label;
    QString subcategory;
};

QList<ResolvedField> resolvedFields(const SettingsGroup *group);

QString label(const QByteArray &propertyName);
}
