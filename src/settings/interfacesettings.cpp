#include "interfacesettings.h"
#include "settingsfactory.h"
#include "settingsio.h"

InterfaceSettings::InterfaceSettings(QObject *parent)
    : SettingsGroup(parent), m_settings(SettingsFactory::create(this)) {
    m_settings->beginGroup(QStringLiteral("interface"));

    m_slidingLabels =
        SettingsIO::readBool(m_settings, QStringLiteral("slidingLabels"), true);
}

QString InterfaceSettings::settingsCategory() const {
    return tr("Interface");
}

QList<SettingsFieldMeta> InterfaceSettings::settingsFields() const {
    return {
        {"slidingLabels", tr("Slide long labels instead of truncating them")},
    };
}

bool InterfaceSettings::slidingLabels() const {
    return m_slidingLabels;
}

void InterfaceSettings::setSlidingLabels(bool newSlidingLabels) {
    if (m_slidingLabels == newSlidingLabels) {
        return;
    }

    m_slidingLabels = newSlidingLabels;
    SettingsIO::write(m_settings, QStringLiteral("slidingLabels"),
                      m_slidingLabels);

    emit slidingLabelsChanged();
}
