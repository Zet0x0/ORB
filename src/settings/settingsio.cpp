#include "settingsio.h"

namespace SettingsIO {
void write(QSettings *settings, const QString &key, const QVariant &value) {
    settings->setValue(key, value);
}

int readInt(QSettings *settings, const QString &key, int defaultValue) {
    bool ok = false;
    const int value = settings->value(key).toInt(&ok);

    return ok ? value : defaultValue;
}

QString readString(QSettings *settings, const QString &key,
                   const QString &defaultValue) {
    return settings->value(key, defaultValue).toString();
}
}
