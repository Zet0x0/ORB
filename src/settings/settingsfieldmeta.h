#pragma once

#include <QString>

struct SettingsFieldMeta {
    QByteArray propertyName;

    QString label;
    // Empty ones fall back to the group's
    QByteArray category;
    QByteArray subcategory;

    // Only for int-typed fields
    int min = 0;
    int max = 99;
};
