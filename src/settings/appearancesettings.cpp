#include "appearancesettings.h"
#include "settingscategories.h"
#include "settingsio.h"

AppearanceSettings::AppearanceSettings(QObject *parent)
    : SettingsGroup(parent, QStringLiteral("appearance")) {
    m_slidingLabels =
        SettingsIO::readBool(m_settings, QStringLiteral("slidingLabels"), true);
}

QList<SettingsFieldMeta> AppearanceSettings::settingsFields() const {
    return {
        {.propertyName = "slidingLabels",
         .label = tr("Slide long labels"),
         .description = tr("Make some long labels, like the station name at "
                           "the top, scroll by instead of getting cut off"),
         .category = SettingsCategory::Appearance,
         .subcategory = SettingsSubcategory::General},
    };
}

bool AppearanceSettings::slidingLabels() const {
    return m_slidingLabels;
}

void AppearanceSettings::setSlidingLabels(bool newSlidingLabels) {
    if (m_slidingLabels == newSlidingLabels) {
        return;
    }

    m_slidingLabels = newSlidingLabels;
    SettingsIO::write(m_settings, QStringLiteral("slidingLabels"),
                      m_slidingLabels);

    emit slidingLabelsChanged();
}
