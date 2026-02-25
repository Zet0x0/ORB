#pragma once

#include "../singleton.h"
#include "source.h"
#include "sourceerror.h"
#include "stationmodel.h"
#include <QQmlEngine>

class SourceController : public QObject, public Singleton<SourceController> {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(SourceController::SearchState searchState READ searchState NOTIFY
                   searchStateChanged FINAL)
    Q_PROPERTY(SourceError error READ error NOTIFY errorChanged FINAL)

    friend class Singleton<SourceController>;

public:
    enum class SearchState { Idle, Searching, Error };
    Q_ENUM(SearchState)

private:
    QHash<QString, Source *> m_sources;
    QStringList m_sourcesInsertOrder;

    Source *m_source = nullptr;
    SearchState m_searchState;
    SourceError m_error;

    StationModel *m_stationModel = nullptr;

    explicit SourceController(QObject *parent = nullptr);

    void undoSourceConnections(Source *source) const;
    void setupSourceConnections(Source *source) const;

    void cancelSearch();
    void setSearchState(const SearchState &newSearchState);

private slots:
    void onSourceStationsDispatched(const QList<Station> &stations);
    void onSearchStarted();
    void onSearchCancelled();

    void setError(const SourceError &error);

public:
    bool sourceExists(const QString &sourceName) const;
    bool registerSource(const QString &name, Source *source);
    Q_INVOKABLE QStringList getSources() const;

    SearchState searchState() const;
    SourceError error() const;

    Q_INVOKABLE StationModel *stationModel() const;

    Q_INVOKABLE bool canShowDefaultStations() const;

public slots:
    void setSource(const QString &newSourceName);
    void search(const QString &query);

    void showDefaultStations();

signals:
    void searchStateChanged();
    void errorChanged();
};
