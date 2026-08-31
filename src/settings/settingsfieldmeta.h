#pragma once

#include <QString>

struct SettingsFieldMeta {
    QByteArray propertyName;

    QString label;
    QString subcategory;

    // Only for int-typed fields
    int min = 0;
    int max = 99;
};
