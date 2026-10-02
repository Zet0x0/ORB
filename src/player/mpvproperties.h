#pragma once

#include <QLatin1StringView>

namespace MpvProperties {
constexpr QLatin1StringView NowPlaying("media-title");
constexpr QLatin1StringView Filename("filename");
constexpr QLatin1StringView Elapsed("time-pos");
constexpr QLatin1StringView Volume("volume");
constexpr QLatin1StringView Mute("mute");
constexpr QLatin1StringView Version("mpv-version");
}
