#include "sourcecontroller.h"

SourceController::SourceController(QObject *parent)
    : QObject(parent), m_currentSearchSource(nullptr) {
    registerSource(tr("Not selected"), nullptr);
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

    if (m_searchState != SourceController::SearchState::Error) {
        setSearchError(QString(), QString());
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

// TODO: handle results
void SourceController::onSearchSuccessful() {
    setSearchState(SourceController::SearchState::Idle);
}

void SourceController::onSearchErrorOccurred(const QString &message) {
    setSearchError(tr("Error"), message);
}

bool SourceController::registerSource(const QString &name, Source *source) {
    if (sourceExists(name)) {
        return false;
    }

    if (source) {
        source->setParent(this);

        connect(source, &Source::searchStarted, this,
                &SourceController::onSearchStarted);
        connect(source, &Source::searchCancelled, this,
                &SourceController::onSearchCancelled);
        connect(source, &Source::searchSuccessful, this,
                &SourceController::onSearchSuccessful);
        connect(source, &Source::searchErrorOccurred, this,
                &SourceController::onSearchErrorOccurred);
    }

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

SourceController::SearchState SourceController::searchState() const {
    return m_searchState;
}

SearchError SourceController::searchError() const {
    return m_searchError;
}

void SourceController::search(const QString &sourceName, QString query) {
    cancelSearch();

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
