#include "sourcecontroller.h"

SourceController::SourceController(QObject *parent)
    : QObject(parent), m_stationModel(new StationModel) {
    setupSourceResultModelConnections();
}

void SourceController::setupSourceResultModelConnections() const {
    connect(m_stationModel, &QAbstractListModel::rowsInserted, this,
            &SourceController::stationModelChanged);
    connect(m_stationModel, &QAbstractListModel::modelReset, this,
            &SourceController::stationModelChanged);
}

void SourceController::setupSourceConnections(Source *source) const {
    connect(source, &Source::searchStarted, this,
            &SourceController::onSearchStarted);
    connect(source, &Source::searchCancelled, this,
            &SourceController::onSearchCancelled);
    connect(source, &Source::searchCompleted, this,
            &SourceController::onSearchCompleted);
    connect(source, &Source::searchErrorOccurred, this,
            &SourceController::onSearchErrorOccurred);
}

void SourceController::setSearchState(
    const SourceController::SearchState &newSearchState) {
    if (m_searchState == newSearchState) {
        return;
    }

    m_searchState = newSearchState;

    if (m_searchState != SourceController::SearchState::Searching) {
        m_currentSearchSource = nullptr;
    }

    emit searchStateChanged();
}

void SourceController::setSearchError(const QString &title,
                                      const QString &message) {
    if (title == m_searchError.title && message == m_searchError.message) {
        return;
    }

    m_searchError.title = title;
    m_searchError.message = message;

    emit searchErrorChanged();

    if (!title.isEmpty() || !message.isEmpty()) {
        setSearchState(SourceController::SearchState::Error);
    }
}

void SourceController::cancelSearch() {
    if (m_currentSearchSource) {
        m_currentSearchSource->cancelSearch();
    }
}

void SourceController::onSearchStarted() {
    setSearchState(SourceController::SearchState::Searching);
}

void SourceController::onSearchCancelled() {
    setSearchState(SourceController::SearchState::Idle);
}

void SourceController::onSearchCompleted(const QList<Station> &stations) {
    m_stationModel->setStations(stations);

    setSearchState(SourceController::SearchState::Idle);
}

void SourceController::onSearchErrorOccurred(const QString &message) {
    setSearchError(tr("Error"), message);
}

bool SourceController::registerSource(const QString &name, Source *source) {
    if (sourceExists(name)) {
        return false;
    }

    source->setParent(this);

    setupSourceConnections(source);

    m_sources[name] = source;
    m_sourcesInsertOrder << name;

    return true;
}

bool SourceController::sourceExists(const QString &sourceName) const {
    return m_sources.contains(sourceName);
}

QStringList SourceController::getSources() const {
    return m_sourcesInsertOrder;
}

bool SourceController::hasDefaultStations(const QString &sourceName) {
    if (!sourceExists(sourceName)) {
        return false;
    }

    return m_sources[sourceName]->hasDefaultStations();
}

SourceController::SearchState SourceController::searchState() const {
    return m_searchState;
}

SearchError SourceController::searchError() const {
    return m_searchError;
}

StationModel *SourceController::stationModel() const {
    return m_stationModel;
}

void SourceController::loadDefaultStations(const QString &sourceName) {
    if (!sourceExists(sourceName) || !hasDefaultStations(sourceName)) {
        return;
    }

    m_sources[sourceName]->loadDefaultStations();
}

void SourceController::search(const QString &sourceName, QString query) {
    cancelSearch();
    setSearchError(QString(), QString());
    m_stationModel->clear();

    if (!sourceExists(sourceName)) {
        setSearchError(tr("Invalid source"),
                       tr("An invalid source identifier was passed"));

        return;
    }

    query = query.trimmed();

    if (query.isEmpty()) {
        // it shouldn't get to this point but let's be precautious
        setSearchError(tr("Nothing to show"), tr("Your search query is empty"));

        return;
    }

    Source *source = m_sources[sourceName];

    m_currentSearchSource = source;
    source->search(query);
}
