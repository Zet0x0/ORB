#pragma once

#include <QString>

struct SettingsFieldMeta {
    QByteArray propertyName;

    QString label;
    // IDs from settingscategories.h
    QByteArray category;
    QByteArray subcategory;

    // Only for int-typed fields
    int min = 0;
    int max = 99;
    int step = 1;
};
