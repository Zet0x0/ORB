#pragma once

#include <QSettings>

namespace SettingsFactory {
QSettings *create(QObject *parent = nullptr);
}
