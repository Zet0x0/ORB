pragma Singleton

import QtQuick

QtObject {
    readonly property ColorGroup active: ColorGroup {
        accent: "#a0f"
        alternateBase: "#303030"
        base: "#242424"
        brightText: "#fff"
        button: "#383838"
        buttonText: "#e0e0e0"
        dark: "#0a0a0a"
        highlight: "#a0f"
        highlightedText: "#fff"
        light: "#707070"
        link: "#b23cf5"
        linkVisited: "#cf88ea"
        mid: "#212121"
        midlight: "#545454"
        placeholderText: "#707070"
        shadow: "#000"
        text: "#e0e0e0"
        toolTipBase: "#333"
        toolTipText: "#fff"
        window: "#121212"
        windowText: "#e0e0e0"
    }
    readonly property ColorGroup disabled: ColorGroup {
        accent: "#550080"
        alternateBase: "#303030"
        base: "#1a1a1a"
        brightText: "#8a8a8a"
        button: "#2c2c2c"
        buttonText: "#6e6e6e"
        dark: "#0a0a0a"
        highlight: "#3d3d3d"
        highlightedText: "#8a8a8a"
        light: "#4a4a4a"
        link: "#5e2680"
        linkVisited: "#8a6a9c"
        mid: "#212121"
        midlight: "#454545"
        placeholderText: "#555"
        shadow: "#000"
        text: "#6e6e6e"
        toolTipBase: "#333"
        toolTipText: "#8a8a8a"
        window: "#121212"
        windowText: "#6e6e6e"
    }
    // Should be identical to `active` group
    readonly property ColorGroup inactive: active
    readonly property SemanticColors semantic: SemanticColors {
        danger: "#f00"
        info: "#007fda"
        success: "#00903c"
        warning: "#ae6e00"
    }

    component SemanticColors: QtObject {
        property color danger
        property color info
        property color success
        property color warning
    }
}
