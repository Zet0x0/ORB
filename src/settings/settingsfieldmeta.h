#pragma once

#include <QString>

struct SettingsFieldMeta {
    QByteArray propertyName;

    QString label;
    QByteArray subcategory;

    // Only for int-typed fields
    int min = 0;
    int max = 99;
};
