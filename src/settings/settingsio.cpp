#include "settingsio.h"
#include "../common/logcategories.h"

namespace {
QString keyPath(const QSettings *settings, const QString &key) {
    return QStringLiteral("%0/%1").arg(settings->group(), key);
}
}

namespace SettingsIO {
void write(QSettings *settings, const QString &key, const QVariant &value) {
    settings->setValue(key, value);
}

int readInt(QSettings *settings, const QString &key, int defaultValue) {
    bool ok = false;
    const int value = settings->value(key, defaultValue).toInt(&ok);

    if (!ok) {
        qCWarning(lcSettings).nospace()
            << keyPath(settings, key) << " is "
            << settings->value(key).toString() << ", not a number, using "
            << defaultValue;

        return defaultValue;
    }

    return value;
}

int readInt(QSettings *settings, const QString &key, int defaultValue, int min,
            int max) {
    const int value = readInt(settings, key, defaultValue);
    const int bounded = qBound(min, value, max);

    if (bounded != value) {
        qCWarning(lcSettings).nospace()
            << keyPath(settings, key) << " is " << value << ", outside " << min
            << ".." << max << ", using " << bounded;
    }

    return bounded;
}

bool readBool(QSettings *settings, const QString &key, bool defaultValue) {
    return settings->value(key, defaultValue).toBool();
}

QString readString(QSettings *settings, const QString &key,
                   const QString &defaultValue) {
    return settings->value(key, defaultValue).toString();
}
}
