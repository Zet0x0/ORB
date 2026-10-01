#include "loggingsettings.h"
#include "settingscategories.h"
#include "settingsio.h"

namespace {
constexpr int MaxEntriesMin = 100;
constexpr int MaxEntriesMax = 20000;
constexpr int MaxFilesMin = 1;
constexpr int MaxFilesMax = 100;
}

LoggingSettings::LoggingSettings(QObject *parent)
    : SettingsGroup(parent, QStringLiteral("logging")) {
    m_maxEntries = SettingsIO::readInt(m_settings, QStringLiteral("maxEntries"),
                                       2000, MaxEntriesMin, MaxEntriesMax);
    m_maxFiles = SettingsIO::readInt(m_settings, QStringLiteral("maxFiles"), 30,
                                     MaxFilesMin, MaxFilesMax);
}

QList<SettingsFieldMeta> LoggingSettings::settingsFields() const {
    return {
        {.propertyName = "maxEntries",
         .label = tr("Messages kept in log dialog"),
         .category = SettingsCategory::Advanced,
         .subcategory = SettingsSubcategory::Logging,
         .min = MaxEntriesMin,
         .max = MaxEntriesMax,
         .step = 100},
        {.propertyName = "maxFiles",
         .label = tr("Log files to keep"),
         .description = tr("How many log files to keep in the logs folder. "
                           "Older ones get cleaned up every time ORB starts, "
                           "and right away when you lower this"),
         .category = SettingsCategory::Advanced,
         .subcategory = SettingsSubcategory::Logging,
         .min = MaxFilesMin,
         .max = MaxFilesMax},
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

int LoggingSettings::maxFiles() const {
    return m_maxFiles;
}

void LoggingSettings::setMaxFiles(int newMaxFiles) {
    newMaxFiles = qBound(MaxFilesMin, newMaxFiles, MaxFilesMax);

    if (m_maxFiles == newMaxFiles) {
        return;
    }

    m_maxFiles = newMaxFiles;
    SettingsIO::write(m_settings, QStringLiteral("maxFiles"), m_maxFiles);

    emit maxFilesChanged();
}
