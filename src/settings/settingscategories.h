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
}

namespace SettingsSubcategory {
inline constexpr char General[] =
    QT_TRANSLATE_NOOP("SettingsCategory", "General");
inline constexpr char Tray[] = QT_TRANSLATE_NOOP("SettingsCategory", "Tray");
inline constexpr char LogMessagesDialog[] =
    QT_TRANSLATE_NOOP("SettingsCategory", "Log Messages Dialog");
}
