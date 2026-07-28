#pragma once

#include "../common/errorinfo.h"
#include "../common/singleton.h"
#include "source.h"
#include "stationmodel.h"
#include <QQmlEngine>

namespace SourceControllerConstants {
constexpr QLatin1StringView NullSourceKey("none");
}

class SourceController : public QObject, public Singleton<SourceController> {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(SourceController::SearchState searchState READ searchState NOTIFY
                   searchStateChanged FINAL)
    Q_PROPERTY(ErrorInfo error READ error NOTIFY errorChanged FINAL)

    Q_PROPERTY(bool canShowDefaultStations READ canShowDefaultStations WRITE
                   setCanShowDefaultStations NOTIFY
                       canShowDefaultStationsChanged FINAL)

    Q_PROPERTY(QString currentSourceUrl READ currentSourceUrl NOTIFY
                   currentSourceUrlChanged FINAL)
    Q_PROPERTY(bool currentSourceIsNull READ currentSourceIsNull NOTIFY
                   currentSourceUrlChanged FINAL)

    friend class Singleton<SourceController>;

public:
    enum class SearchState { Idle, Searching, Error };
    Q_ENUM(SearchState)

private:
    QHash<QString, Source *> m_sources;
    QHash<QString, QString> m_sourceDisplayNames;
    QStringList m_sourcesInsertOrder;

    Source *m_source = nullptr;
    QString m_currentSourceKey;
    SearchState m_searchState;
    ErrorInfo m_error;

    StationModel *m_stationModel = nullptr;

    bool m_canShowDefaultStations = false;

    explicit SourceController(QObject *parent = nullptr);

    void undoSourceConnections(Source *source) const;
    void setupSourceConnections(Source *source) const;

    void cancelSearch();
    void setSearchState(const SearchState &newSearchState);

    void setCanShowDefaultStations(bool newCanShowDefaultStations);

private slots:
    void onSourceStationsDispatched(const QList<Station> &stations);
    void onSearchStarted();
    void onSearchCancelled();

    void setError(const ErrorInfo &error);

public:
    bool sourceExists(const QString &key) const;
    bool registerSource(const QString &key, const QString &displayName,
                        Source *source);
    Q_INVOKABLE QVariantList getSources() const;

    SearchState searchState() const;
    ErrorInfo error() const;

    Q_INVOKABLE StationModel *stationModel() const;

    bool canShowDefaultStations() const;

    QString currentSourceUrl() const;
    bool currentSourceIsNull() const;

public slots:
    void setSource(const QString &newSourceName);
    void search(const QString &query);

    void showDefaultStations();

signals:
    void searchStateChanged();
    void errorChanged();

    void canShowDefaultStationsChanged();
    void currentSourceUrlChanged();
};
