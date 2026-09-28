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

    m_maxEntries = SettingsIO::readInt(m_settings, QStringLiteral("maxEntries"),
                                       2000, MaxEntriesMin, MaxEntriesMax);
}

QByteArray LoggingSettings::settingsCategory() const {
    return QT_TRANSLATE_NOOP("SettingsCategory", "Logging");
}

QList<SettingsFieldMeta> LoggingSettings::settingsFields() const {
    return {
        {"maxEntries", tr("Maximum lines shown in log dialog"),
         QT_TRANSLATE_NOOP("SettingsCategory", "Interface"), MaxEntriesMin,
         MaxEntriesMax},
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
