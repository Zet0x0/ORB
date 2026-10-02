pragma ComponentBehavior: Bound

import ORB.Style
import QtQuick

Slider {
    id: root

    property int decimals: 0
    // distance between ticks in value units, like QSlider::tickInterval
    // by default - a quarter of the range rounded up to: 1, 2, 2.5 or 5 * 10^n steps
    property real tickInterval: {
        const range = Math.abs(to - from);
        const step = stepSize > 0 ? stepSize : Math.pow(10, Math.floor(Math.log10(range)) - 2);
        const steps = Math.max(1, range / step / 4);
        const magnitude = Math.pow(10, Math.floor(Math.log10(steps)));
        const fraction = steps / magnitude;
        const multiple = fraction <= 1 ? 1 : fraction <= 2 ? 2 : fraction <= 2.5 && magnitude > 1 ? 2.5 : fraction <= 5 ? 5 : 10;

        return multiple * magnitude * step;
    }

    bottomPadding: verticalPadding + ticks.overhang
    orientation: Qt.Horizontal // the only supported orientation

    topPadding: verticalPadding + (ticks.count > 0 ? fontMetrics.height + ticks.labelSpacing + ticks.overhang : 0)

    FontMetrics {
        id: fontMetrics

        font: root.font
    }

    ToolTip {
        parent: root.handle
        text: ticks.labelText(root.value)
        visible: root.pressed || root.hovered || root.visualFocus
    }

    Repeater {
        id: ticks

        readonly property int first: Math.floor(minimum / root.tickInterval)
        // label only every n-th multiple when the widest label doesn't fit between two ticks
        readonly property int labelEvery: Math.ceil((Math.max(labelWidth(root.from), labelWidth(root.to)) + fontMetrics.averageCharacterWidth) / spacing)
        // spacing between the labels and the tops of the ticks
        readonly property int labelSpacing: 5
        readonly property int last: Math.ceil(maximum / root.tickInterval)
        readonly property real maximum: Math.max(root.from, root.to)
        readonly property real minimum: Math.min(root.from, root.to)
        // how far the ticks stick out above and below the handle
        readonly property int overhang: Math.max(0, Math.ceil((tickHeight - root.implicitHandleHeight) / 2))
        // distance between ticks, in pixels
        readonly property real spacing: (root.availableWidth - root.implicitHandleWidth) * root.tickInterval / (maximum - minimum)
        readonly property int tickHeight: 19

        // centered on the tick, but contained inside root (the slider)
        // although if the two end labels don't fit side by side - they're pushed out
        function labelCenter(value: real): real {
            const spill = Math.max(0, labelWidth(root.from) + labelWidth(root.to) + fontMetrics.averageCharacterWidth - root.width) / 2;
            const x = spill > 0 ? (tickX(value) < root.width / 2 ? -Infinity : Infinity) : tickX(value);

            return Math.max(labelWidth(value) / 2 - spill, Math.min(x, root.width - labelWidth(value) / 2 + spill));
        }

        function labelText(value: real): string {
            return root.locale.toString(value, "f", root.decimals);
        }

        function labelWidth(value: real): real {
            return fontMetrics.advanceWidth(labelText(value));
        }

        // whether the labels of a and b values are closer than a character apart
        function overlap(a: real, b: real): bool {
            return Math.abs(labelCenter(a) - labelCenter(b)) < ((labelWidth(a) + labelWidth(b)) / 2 + fontMetrics.averageCharacterWidth);
        }

        function tickX(value: real): real {
            const position = root.to === root.from ? 0 : (value - root.from) / (root.to - root.from);

            return root.leftPadding + root.implicitHandleWidth / 2 + (root.mirrored ? 1 - position : position) * (root.availableWidth - root.implicitHandleWidth);
        }

        model: maximum > minimum ? (spacing >= 6 ? last - first + 1 : 2) : 1

        delegate: Item {
            id: tick

            required property int index
            readonly property real value: index === 0 ? ticks.minimum : (index === ticks.count - 1 ? ticks.maximum : (ticks.first + index) * root.tickInterval)

            z: -2

            Rectangle {
                id: tickLine

                color: root.palette.button
                height: ticks.tickHeight
                width: 2
                x: Math.round(ticks.tickX(tick.value)) - 1
                y: Math.round(root.topPadding + (root.availableHeight - height) / 2)
            }

            Label {
                text: ticks.labelText(tick.value)
                visible: tick.value === ticks.minimum || tick.value === ticks.maximum || (ticks.first + tick.index) % ticks.labelEvery === 0 && !ticks.overlap(tick.value, root.from) && !ticks.overlap(tick.value, root.to)
                x: Math.round(ticks.labelCenter(tick.value) - implicitWidth / 2)
                y: tickLine.y - ticks.labelSpacing - implicitHeight
            }
        }
    }
}
