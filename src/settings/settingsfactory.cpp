#include "settingsfactory.h"

namespace SettingsFactory {
QSettings *create(QObject *parent) {
    return new QSettings(QStringLiteral("settings.ini"), QSettings::IniFormat,
                         parent);
}
}
