#include "sourcecontroller.h"

SourceController::SourceController(QObject *parent)
    : QObject(parent), m_stationModel(new StationModel) {
    setupStationModelConnections();
}

void SourceController::setupStationModelConnections() const {
    connect(m_stationModel, &QAbstractListModel::rowsInserted, this,
            &SourceController::stationModelChanged);
    connect(m_stationModel, &QAbstractListModel::modelReset, this,
            &SourceController::stationModelChanged);
}

void SourceController::undoSourceConnections(Source *source) const {
    disconnect(source, nullptr, this, nullptr);
}

void SourceController::setupSourceConnections(Source *source) const {
    connect(source, &Source::stationsDispatched, this,
            &SourceController::onSourceStationsDispatched);
    connect(source, &Source::errorOccurred, this, &SourceController::setError);

    connect(source, &Source::searchStarted, this,
            &SourceController::onSearchStarted);
    connect(source, &Source::searchCancelled, this,
            &SourceController::onSearchCancelled);
}

void SourceController::cancelSearch() {
    m_source->cancelSearch();
    setSearchState(SearchState::Idle);
}

void SourceController::setSearchState(const SearchState &newSearchState) {
    if (m_searchState == newSearchState) {
        return;
    }

    if (m_searchState == SearchState::Error) {
        setError(SourceError{});
    }

    m_searchState = newSearchState;

    emit searchStateChanged();
}

// TODO: account for stations dispatching from a loadDefaultStations call
void
SourceController::onSourceStationsDispatched(const QList<Station> &stations) {
    m_stationModel->setStations(stations);

    setSearchState(SearchState::Idle);
}

void SourceController::onSearchStarted() {
    setSearchState(SearchState::Searching);
}

void SourceController::onSearchCancelled() {
    setSearchState(SearchState::Idle);
}

void SourceController::setError(const SourceError &error) {
    if (m_error == error) {
        return;
    }

    m_error = error;

    emit errorChanged();

    if (!m_error.title.isEmpty() || !m_error.message.isEmpty()) {
        setSearchState(SearchState::Error);
    }
}

bool SourceController::sourceExists(const QString &sourceName) const {
    return m_sources.contains(sourceName);
}

bool SourceController::registerSource(const QString &name, Source *source) {
    if (sourceExists(name)) {
        return false;
    }

    source->setParent(this);

    m_sources[name] = source;
    m_sourcesInsertOrder << name;

    if (!m_source) {
        setSource(name);
    }

    return true;
}

QStringList SourceController::getSources() const {
    return m_sourcesInsertOrder;
}

SourceController::SearchState SourceController::searchState() const {
    return m_searchState;
}

SourceError SourceController::error() const {
    return m_error;
}

StationModel *SourceController::stationModel() const {
    return m_stationModel;
}

bool SourceController::canShowDefaultStations() const {
    return m_source->hasDefaultStations();
}

void SourceController::setSource(const QString &newSourceName) {
    Source *newSource = m_sources.value(newSourceName, nullptr);

    if (m_source == newSource) {
        return;
    }

    if (m_source) {
        cancelSearch();
        undoSourceConnections(m_source);

        m_stationModel->clear();
    }

    setupSourceConnections(newSource);

    m_source = newSource;
}

void SourceController::search(const QString &query) {
    cancelSearch();

    m_source->search(query);
}

void SourceController::showDefaultStations() {
    cancelSearch();

    if (!canShowDefaultStations()) {
        return;
    }

    m_source->loadDefaultStations();
}
