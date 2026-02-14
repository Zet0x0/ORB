#pragma once

#include "../player/stationModel.h"
#include "../singleton.h"
#include "searcherror.h"
#include "source.h"
#include <QQmlEngine>

class SourceController : public QObject, public Singleton<SourceController> {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(SourceController::SearchState searchState READ searchState NOTIFY
                   searchStateChanged FINAL)
    Q_PROPERTY(SearchError searchError READ searchError NOTIFY
                   searchErrorChanged FINAL)
    Q_PROPERTY(StationModel *stationModel READ stationModel NOTIFY
                   stationModelChanged FINAL)

    friend class Singleton<SourceController>;

public:
    enum class SearchState { Idle, Searching, Error };
    Q_ENUM(SearchState)

private:
    QHash<QString, Source *> m_sources;
    QStringList m_sourcesInsertOrder;

    Source *m_currentSearchSource = nullptr;
    SearchState m_searchState;
    SearchError m_searchError;
    StationModel *m_stationModel = nullptr;

    explicit SourceController(QObject *parent = nullptr);

    void setupSourceResultModelConnections() const;
    void setupSourceConnections(Source *source) const;

    void setSearchState(const SearchState &newSearchState);
    void setSearchError(const QString &title, const QString &message);

    void cancelSearch();

private slots:
    void onSourceStationsDispatched(const QList<Station> &stations);
    void onSourceErrorOccurred(const QString &message);

    void onSearchStarted();
    void onSearchCancelled();

public:
    bool registerSource(const QString &name, Source *source);
    bool sourceExists(const QString &sourceName) const;
    Q_INVOKABLE QStringList getSources() const;

    Q_INVOKABLE bool hasDefaultStations(const QString &sourceName);

    SearchState searchState() const;
    SearchError searchError() const;
    StationModel *stationModel() const;

public slots:
    void loadDefaultStations(const QString &sourceName);

    void search(const QString &sourceName, QString query);

signals:
    void searchStateChanged();
    void searchErrorChanged();
    void stationModelChanged();
};
