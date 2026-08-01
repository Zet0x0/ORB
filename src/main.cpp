#include "sources/providers/nullsource.h"
#include "sources/providers/radiorecord.h"
#include "sources/sourcecontroller.h"
#include <QGuiApplication>
#include <QIcon>
#include <QPalette>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

namespace {
struct ColorRoleMapping {
    QPalette::ColorRole role;
    const char *propertyName;
};

constexpr ColorRoleMapping colorRoleMappings[] = {
    {QPalette::Accent, "accent"},
    {QPalette::AlternateBase, "alternateBase"},
    {QPalette::Base, "base"},
    {QPalette::BrightText, "brightText"},
    {QPalette::Button, "button"},
    {QPalette::ButtonText, "buttonText"},
    {QPalette::Dark, "dark"},
    {QPalette::Highlight, "highlight"},
    {QPalette::HighlightedText, "highlightedText"},
    {QPalette::Light, "light"},
    {QPalette::Link, "link"},
    {QPalette::LinkVisited, "linkVisited"},
    {QPalette::Mid, "mid"},
    {QPalette::Midlight, "midlight"},
    {QPalette::PlaceholderText, "placeholderText"},
    {QPalette::Shadow, "shadow"},
    {QPalette::Text, "text"},
    {QPalette::ToolTipBase, "toolTipBase"},
    {QPalette::ToolTipText, "toolTipText"},
    {QPalette::Window, "window"},
    {QPalette::WindowText, "windowText"},
};

void applyColorGroup(QPalette &palette, QPalette::ColorGroup group,
                     QObject *colorGroup) {
    for (const ColorRoleMapping &mapping : colorRoleMappings) {
        palette.setColor(
            group, mapping.role,
            colorGroup->property(mapping.propertyName).value<QColor>());
    }
}

QPalette paletteFromQmlPalette(QObject *qmlPalette) {
    QPalette palette;

    applyColorGroup(palette, QPalette::Active,
                    qmlPalette->property("active").value<QObject *>());
    applyColorGroup(palette, QPalette::Inactive,
                    qmlPalette->property("inactive").value<QObject *>());
    applyColorGroup(palette, QPalette::Disabled,
                    qmlPalette->property("disabled").value<QObject *>());

    return palette;
}
}

int main(int argc, char *argv[]) {
    QCoreApplication::setOrganizationName(QStringLiteral("zet0x0"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("com.zet0x0.orb"));
    QCoreApplication::setApplicationName(QStringLiteral("ORB"));
    QCoreApplication::setApplicationVersion(QStringLiteral("0.1.0"));

    QGuiApplication::setQuitOnLastWindowClosed(false);

    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    QIcon::setThemeName("ORB");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [&app](QObject *object, const QUrl &) {
            if (object) {
                app.setPalette(paletteFromQmlPalette(
                    object->property("palette").value<QObject *>()));
            }
        },
        Qt::QueuedConnection);

    // Add sources
    {
        SourceController *sourceController = SourceController::instance();

        sourceController->registerSource(
            SourceControllerConstants::NullSourceKey.toString(),
            QObject::tr("Not selected"), new NullSource);
        sourceController->registerSource(QStringLiteral("radio-record"),
                                         QObject::tr("Radio Record"),
                                         new RadioRecord);
    }

    engine.loadFromModule(QStringLiteral("ORB"), QStringLiteral("Main"));

    return app.exec();
}
