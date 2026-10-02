#pragma once

#include "settingsgroup.h"
#include <QByteArray>
#include <QList>
#include <QMetaProperty>
#include <QString>

namespace SettingsIntrospection {
struct ResolvedField {
    QMetaProperty property;
    QString label;
    QString description;
    QByteArray categoryId;
    QByteArray subcategoryId;
    // Translated for display
    QString subcategory;

    int min;
    int max;
    int step;
};

// Groups exposed by the Settings singleton, in declared order
QList<SettingsGroup *> groups();

QList<ResolvedField> resolvedFields(const SettingsGroup *group);

QString label(const QByteArray &propertyName);

QString categoryName(const QByteArray &id);
}
