#pragma once

#include <QSettings>

namespace SettingsIO {
int readInt(QSettings *settings, const QString &key, int defaultValue);
void writeInt(QSettings *settings, const QString &key, int value);
}
