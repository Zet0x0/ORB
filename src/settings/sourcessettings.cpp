#include "sourcessettings.h"
#include "settingsfactory.h"
#include "settingsio.h"

SourcesSettings::SourcesSettings(QObject *parent)
    : SettingsGroup(parent), m_settings(SettingsFactory::create(this)) {
    m_settings->beginGroup(QStringLiteral("sources"));

    m_lastSearchSource = SettingsIO::readString(
        m_settings, QStringLiteral("lastSearchSource"), QStringLiteral("none"));
}

QString SourcesSettings::lastSearchSource() const {
    return m_lastSearchSource;
}

void SourcesSettings::setLastSearchSource(const QString &newLastSearchSource) {
    if (m_lastSearchSource == newLastSearchSource) {
        return;
    }

    m_lastSearchSource = newLastSearchSource;
    SettingsIO::write(m_settings, QStringLiteral("lastSearchSource"),
                      m_lastSearchSource);

    emit lastSearchSourceChanged();
}
