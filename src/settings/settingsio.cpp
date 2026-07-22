#include "settingsio.h"

namespace SettingsIO {
int readInt(QSettings *settings, const QString &key, int defaultValue) {
    bool ok = false;
    const int value = settings->value(key).toInt(&ok);

    return ok ? value : defaultValue;
}

void writeInt(QSettings *settings, const QString &key, int value) {
    settings->setValue(key, value);
}
}
