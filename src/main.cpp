#include "common/logcategories.h"
#include "logging/logger.h"
#include "orbversion.h"
#include "settings/loggingsettings.h"
#include "settings/settings.h"
#include "sources/providers/favoritessource.h"
#include "sources/providers/nullsource.h"
#include "sources/providers/radiorecord.h"
#include "sources/sourcecontroller.h"
#include <QByteArray>
#include <QColor>
#include <QGuiApplication>
#include <QIcon>
#include <QLocale>
#include <QMetaEnum>
#include <QPalette>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QSGRendererInterface>
#include <QStandardPaths>
#include <QString>
#include <QSysInfo>
#include <QTranslator>
#include <QUrl>
#include <QVariant>
#include <cctype>
#include <memory>

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
    QCoreApplication::setApplicationName(QStringLiteral("ORB"));
    QCoreApplication::setApplicationVersion(QStringLiteral(ORB_VERSION_STRING));
    QCoreApplication::setOrganizationDomain(QStringLiteral("zet0x0.com"));

    Logger::install();

    QGuiApplication::setQuitOnLastWindowClosed(false);

    QQuickStyle::setStyle(QStringLiteral("ORB.Style"));
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QIcon::setThemeName(QStringLiteral("ORB"));

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qCInfo(lcApp).noquote()
        << QStringLiteral("ORB %0 on %1, Qt %2 (%3 platform)")
               .arg(QCoreApplication::applicationVersion(),
                    QSysInfo::prettyProductName(),
                    QString::fromLatin1(qVersion()),
                    QGuiApplication::platformName());
    qCInfo(lcApp) << "Data directory:"
                  << QStandardPaths::writableLocation(
                         QStandardPaths::AppDataLocation);

    QTranslator pluralsTranslator;
    QTranslator translator;

    if (pluralsTranslator.load(QStringLiteral(":/i18n/ORB_en"))) {
        QCoreApplication::installTranslator(&pluralsTranslator);
    } else {
        qCWarning(lcApp) << "Failed to load :/i18n/ORB_en, English plural "
                            "forms will be wrong";
    }

    const bool hasTranslation =
        translator.load(QLocale(), QStringLiteral("ORB"), QStringLiteral("_"),
                        QStringLiteral(":/i18n")) &&
        translator.filePath() != pluralsTranslator.filePath();

    if (hasTranslation) {
        QCoreApplication::installTranslator(&translator);
    }

    qCInfo(lcApp)
        << "Using translation"
        << (hasTranslation ? translator : pluralsTranslator).filePath() << "for"
        << QLocale().uiLanguages();

    {
        LoggingSettings *loggingSettings = Settings::instance()->logging();
        Logger *logger = Logger::instance();

        logger->setMaxEntries(loggingSettings->maxEntries());
        logger->setMaxFiles(loggingSettings->maxFiles());
        Logger::setFilterRules(loggingSettings->filterRules());

        QObject::connect(loggingSettings, &LoggingSettings::maxEntriesChanged,
                         logger, [loggingSettings, logger]() {
                             logger->setMaxEntries(
                                 loggingSettings->maxEntries());
                         });
        QObject::connect(loggingSettings, &LoggingSettings::maxFilesChanged,
                         logger, [loggingSettings, logger]() {
                             logger->setMaxFiles(loggingSettings->maxFiles());
                         });
        QObject::connect(loggingSettings, &LoggingSettings::filterRulesChanged,
                         logger, [loggingSettings]() {
                             Logger::setFilterRules(
                                 loggingSettings->filterRules());
                         });
    }

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        [](const QUrl &url) {
            qCCritical(lcApp)
                << "Failed to create" << url.toDisplayString() << "- exiting";

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
            } else {
                qCWarning(lcApp) << "Failed to create ORB.Style/AppPalette:"
                                 << paletteComponent.errors();
            }
        } else {
            qCWarning(lcApp) << "Failed to load ORB.Style/AppPalette:"
                             << paletteComponent.errors();
        }
    }

    // Add sources
    {
        SourceController *sourceController = SourceController::instance();

        sourceController->registerSource(
            SourceControllerConstants::NullSourceKey.toString(),
            QCoreApplication::translate("main", "Not selected"),
            new NullSource);
        sourceController->registerSource(
            QStringLiteral("favorites"),
            QCoreApplication::translate("main", "Favorites"),
            new FavoritesSource);
        sourceController->registerSource(QStringLiteral("radio-record"),
                                         QStringLiteral("Radio Record"),
                                         new RadioRecord);
    }

    engine.loadFromModule(QStringLiteral("ORB"), QStringLiteral("Main"));

    const int exitCode = app.exec();

    qCInfo(lcApp) << "Exiting with code" << exitCode;

    return exitCode;
}
