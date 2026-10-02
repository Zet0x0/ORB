#include "sourcessettings.h"
#include "../sources/sourcecontroller.h"
#include "settingsio.h"

SourcesSettings::SourcesSettings(QObject *parent)
    : SettingsGroup(parent, QStringLiteral("sources")) {
    m_lastSearchSource = SettingsIO::readString(
        m_settings, QStringLiteral("lastSearchSource"),
        SourceControllerConstants::NullSourceKey.toString());
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
