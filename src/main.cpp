#include "sources/providers/favoritessource.h"
#include "sources/providers/nullsource.h"
#include "sources/providers/radiorecord.h"
#include "sources/sourcecontroller.h"
#include <QGuiApplication>
#include <QIcon>
#include <QPalette>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QSettings>
#include <cctype>

namespace {
void applyColorGroup(QPalette &palette, QPalette::ColorGroup group,
                     QObject *colorGroup) {
    static const QMetaEnum roleEnum =
        QMetaEnum::fromType<QPalette::ColorRole>();

    for (int i = 0; i < roleEnum.keyCount(); ++i) {
        QByteArray propertyName = roleEnum.key(i);

        propertyName[0] = static_cast<char>(
            std::tolower(static_cast<unsigned char>(propertyName[0])));

        const QVariant value = colorGroup->property(propertyName.constData());

        if (!value.canConvert<QColor>()) {
            continue;
        }

        palette.setColor(group,
                         static_cast<QPalette::ColorRole>(roleEnum.value(i)),
                         value.value<QColor>());
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

    QSettings::setDefaultFormat(QSettings::IniFormat);

    QGuiApplication::setQuitOnLastWindowClosed(false);

    QQuickStyle::setStyle(QStringLiteral("ORB.Style"));
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

    {
        QQmlComponent paletteComponent(&engine, QStringLiteral("ORB.Style"),
                                       QStringLiteral("AppPalette"));

        if (paletteComponent.isReady()) {
            std::unique_ptr<QObject> paletteObject(paletteComponent.create());

            if (paletteObject) {
                app.setPalette(paletteFromQmlPalette(paletteObject.get()));
            }
        } else {
            qWarning() << "Failed to load ORB.Style/AppPalette:"
                       << paletteComponent.errors();
        }
    }

    // Add sources
    {
        SourceController *sourceController = SourceController::instance();

        sourceController->registerSource(
            SourceControllerConstants::NullSourceKey.toString(),
            QObject::tr("Not selected"), new NullSource);
        sourceController->registerSource(QStringLiteral("favorites"),
                                         QObject::tr("Favorites"),
                                         new FavoritesSource);
        sourceController->registerSource(QStringLiteral("radio-record"),
                                         QObject::tr("Radio Record"),
                                         new RadioRecord);
    }

    engine.loadFromModule(QStringLiteral("ORB"), QStringLiteral("Main"));

    return app.exec();
}
