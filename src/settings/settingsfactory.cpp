#include "settingsfactory.h"

namespace SettingsFactory {
QSettings *create(QObject *parent) {
    return new QSettings(parent);
}
}
