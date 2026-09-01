#include "sourcecontroller.h"

SourceController::SourceController(QObject *parent)
    : QObject(parent), m_stationModel(new StationModel(this)) {}

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
    if (!m_source) {
        return;
    }

    m_source->cancelSearch();
    setSearchState(SearchState::Idle);
}

void SourceController::setSearchState(const SearchState &newSearchState) {
    if (m_searchState == newSearchState) {
        return;
    }

    if (m_searchState == SearchState::Error) {
        setError(ErrorInfo{});
    }

    m_searchState = newSearchState;

    emit searchStateChanged();
}

void
SourceController::setCanShowDefaultStations(bool newCanShowDefaultStations) {
    if (m_canShowDefaultStations == newCanShowDefaultStations) {
        return;
    }

    m_canShowDefaultStations = newCanShowDefaultStations;

    emit canShowDefaultStationsChanged();
}

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

void SourceController::setError(const ErrorInfo &error) {
    if (m_error == error) {
        return;
    }

    m_error = error;

    emit errorChanged();

    if (!m_error.title.isEmpty() || !m_error.message.isEmpty()) {
        setSearchState(SearchState::Error);
    }
}

bool SourceController::sourceExists(const QString &key) const {
    return m_sources.contains(key);
}

bool SourceController::registerSource(const QString &key,
                                      const QString &displayName,
                                      Source *source) {
    if (sourceExists(key)) {
        return false;
    }

    source->setParent(this);

    m_sources[key] = source;
    m_sourceDisplayNames[key] = displayName;
    m_sourcesInsertOrder << key;

    if (!m_source) {
        setSource(key);
    }

    return true;
}

QVariantList SourceController::getSources() const {
    QVariantList sources;

    for (const QString &key : m_sourcesInsertOrder) {
        sources << QVariantMap{
            {QStringLiteral("key"), key},
            {QStringLiteral("name"), m_sourceDisplayNames.value(key)}};
    }

    return sources;
}

SourceController::SearchState SourceController::searchState() const {
    return m_searchState;
}

ErrorInfo SourceController::error() const {
    return m_error;
}

StationModel *SourceController::stationModel() const {
    return m_stationModel;
}

bool SourceController::canShowDefaultStations() const {
    return m_canShowDefaultStations;
}

QString SourceController::currentSourceUrl() const {
    return m_source ? m_source->websiteUrl() : QString();
}

bool SourceController::currentSourceIsNull() const {
    return m_currentSourceKey.isEmpty() ||
           m_currentSourceKey ==
               SourceControllerConstants::NullSourceKey.toString();
}

void SourceController::setSource(const QString &newSourceName) {
    QString resolvedKey = newSourceName;
    Source *newSource = m_sources.value(resolvedKey, nullptr);

    if (!newSource) {
        resolvedKey = SourceControllerConstants::NullSourceKey.toString();
        newSource = m_sources.value(resolvedKey, nullptr);
    }

    if (!newSource || m_source == newSource) {
        return;
    }

    if (m_source) {
        cancelSearch();
        undoSourceConnections(m_source);

        m_stationModel->clear();
    }

    setupSourceConnections(newSource);
    setCanShowDefaultStations(newSource->hasDefaultStations());

    m_source = newSource;
    m_currentSourceKey = resolvedKey;

    emit currentSourceUrlChanged();
}

void SourceController::search(const QString &query) {
    cancelSearch();

    if (!m_source) {
        return;
    }

    m_source->search(query);
}

void SourceController::showDefaultStations() {
    cancelSearch();

    if (!m_source || !canShowDefaultStations()) {
        return;
    }

    m_source->loadDefaultStations();
}
