#include "loggingsettings.h"
#include "settingscategories.h"
#include "settingsio.h"

namespace {
constexpr int MaxEntriesMin = 100;
constexpr int MaxEntriesMax = 20000;
}

LoggingSettings::LoggingSettings(QObject *parent)
    : SettingsGroup(parent, QStringLiteral("logging")) {
    m_maxEntries = SettingsIO::readInt(m_settings, QStringLiteral("maxEntries"),
                                       2000, MaxEntriesMin, MaxEntriesMax);
}

QList<SettingsFieldMeta> LoggingSettings::settingsFields() const {
    return {
        {.propertyName = "maxEntries",
         .label = tr("Messages to show"),
         .category = SettingsCategory::Appearance,
         .subcategory = SettingsSubcategory::LogMessagesDialog,
         .min = MaxEntriesMin,
         .max = MaxEntriesMax,
         .step = 100},
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
