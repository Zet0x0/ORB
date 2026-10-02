#pragma once

#include <QtTranslation>

// the text is used as ID for the "SettingsCategory" context
namespace SettingsCategory {
inline constexpr char Appearance[] =
    QT_TRANSLATE_NOOP("SettingsCategory", "Appearance");
inline constexpr char Playback[] =
    QT_TRANSLATE_NOOP("SettingsCategory", "Playback");
inline constexpr char System[] =
    QT_TRANSLATE_NOOP("SettingsCategory", "System");
inline constexpr char Advanced[] =
    QT_TRANSLATE_NOOP("SettingsCategory", "Advanced");

// order in the preferences dialog, fields of a category
// not listed here are not shown
inline constexpr const char *Order[] = {Appearance, Playback, System, Advanced};
}

namespace SettingsSubcategory {
inline constexpr char General[] =
    QT_TRANSLATE_NOOP("SettingsCategory", "General");
inline constexpr char Tray[] = QT_TRANSLATE_NOOP("SettingsCategory", "Tray");
inline constexpr char Logging[] =
    QT_TRANSLATE_NOOP("SettingsCategory", "Logging");
}
