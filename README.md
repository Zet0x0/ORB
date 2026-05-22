# Unnamed Radio App

## Supported Platforms

- Windows
- ~~Linux~~ TBA
- ~~MacOS~~ No

## Building Prerequisites

- [`Qt`](https://www.qt.io/development/download-open-source) (MinGW) >= 6.10 with following Additional Libraries: `Qt Image Formats`
- [`MpvQt`](https://invent.kde.org/libraries/mpvqt) >= 1.1.1 (_pulled automatically by git submodule_)
- [`Extra CMake Modules`](https://invent.kde.org/frameworks/extra-cmake-modules) >= 6.15.0

### `Extra CMake Modules` on Windows

`Extra CMake Modules` is obtainable using KDE's [`Craft`](https://develop.kde.org/docs/getting-started/building/craft/#craft-setup-on-windows)

Follow the entire **Craft setup on Windows** section, then execute `craft extra-cmake-modules` in your command line to install ECM

Then set the `ECM_DIR` variable for cmake to be equal to where your ECM cmake files (`ECMConfig.cmake`, `ECMConfigVersion.cmake`) are located at - by default it's `C:\CraftRoot\share\ECM\cmake`
