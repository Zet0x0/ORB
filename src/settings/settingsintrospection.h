#pragma once

#include "settingsgroup.h"
#include <QMetaProperty>

namespace SettingsIntrospection {
struct ResolvedField {
    QMetaProperty property;
    QString label;
    // Translated for display
    QString subcategory;

    int min;
    int max;
};

QList<ResolvedField> resolvedFields(const SettingsGroup *group);

QString label(const QByteArray &propertyName);

QString categoryName(const QByteArray &id);
}
