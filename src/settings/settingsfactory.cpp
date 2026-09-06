#include "settingsfactory.h"
#include <QDir>
#include <QStandardPaths>

namespace SettingsFactory {
QSettings *create(QObject *parent) {
    return new QSettings(
        QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))
            .filePath(QStringLiteral("settings.ini")),
        QSettings::IniFormat, parent);
}
}
