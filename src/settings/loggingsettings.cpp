#include "loggingsettings.h"
#include "settingsfactory.h"
#include "settingsio.h"

namespace {
constexpr int MaxEntriesMin = 100;
constexpr int MaxEntriesMax = 100000;
}

LoggingSettings::LoggingSettings(QObject *parent)
    : SettingsGroup(parent), m_settings(SettingsFactory::create(this)) {
    m_settings->beginGroup(QStringLiteral("logging"));

    m_maxEntries = qBound(
        MaxEntriesMin,
        SettingsIO::readInt(m_settings, QStringLiteral("maxEntries"), 2000),
        MaxEntriesMax);
}

QString LoggingSettings::settingsCategory() const {
    return tr("System");
}

QString LoggingSettings::settingsSubcategory() const {
    return tr("Logging");
}

QList<SettingsFieldMeta> LoggingSettings::settingsFields() const {
    return {
        {"maxEntries", tr("Maximum lines shown in log dialog"), QString(),
         MaxEntriesMin, MaxEntriesMax},
    };
}

int LoggingSettings::maxEntries() const {
    return m_maxEntries;
}

void LoggingSettings::setMaxEntries(int newMaxEntries) {
    newMaxEntries = qBound(MaxEntriesMin, newMaxEntries, MaxEntriesMax);

    if (m_maxEntries == newMaxEntries) {
        return;
    }

    m_maxEntries = newMaxEntries;
    SettingsIO::write(m_settings, QStringLiteral("maxEntries"), m_maxEntries);

    emit maxEntriesChanged();
}
