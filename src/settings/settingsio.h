#pragma once

#include <QSettings>

namespace SettingsIO {
void write(QSettings *settings, const QString &key, const QVariant &value);

int readInt(QSettings *settings, const QString &key, int defaultValue);
int readInt(QSettings *settings, const QString &key, int defaultValue, int min,
            int max);

bool readBool(QSettings *settings, const QString &key, bool defaultValue);
QString readString(QSettings *settings, const QString &key,
                   const QString &defaultValue);
}
