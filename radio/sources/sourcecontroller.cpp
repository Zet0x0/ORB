#include "sourcecontroller.h"

SourceController::SourceController(QObject *parent) : QObject(parent) {
    registerSource(tr("Not selected"), nullptr);
}

void SourceController::setSearchState(
    const SourceController::SearchState &newSearchState) {
    if (m_searchState == newSearchState) {
        return;
    }

    m_searchState = newSearchState;

    emit searchStateChanged();
}

void SourceController::setSearchError(const QString &title,
                                      const QString &message) {
    if (title == m_searchError.title || message == m_searchError.message) {
        return;
    }

    m_searchError.title = title;
    m_searchError.message = message;

    emit searchErrorChanged();

    setSearchState(SourceController::SearchState::Error);
}

// TODO
void SourceController::cancelSearch() {
    if (m_searchState != SourceController::SearchState::Searching) {
        return;
    }
}

void SourceController::clearSearchError() {
    if (m_searchState != SourceController::SearchState::Error) {
        return;
    }

    setSearchState(SourceController::SearchState::Idle);
    setSearchError(QString(), QString());
}

bool SourceController::registerSource(const QString &name, Source *source) {
    if (sourceExists(name)) {
        return false;
    }

    if (source) {
        source->setParent(this);
    }

    m_sources.insert(name, source);

    return true;
}

bool SourceController::sourceExists(const QString &sourceName) const {
    return m_sources.contains(sourceName);
}

QStringList SourceController::getSources() const {
    return m_sources.keys();
}

SourceController::SearchState SourceController::searchState() const {
    return m_searchState;
}

SearchError SourceController::searchError() const {
    return m_searchError;
}

// TODO
void SourceController::search(const QString &sourceName, QString query) {
    cancelSearch();
    clearSearchError();

    query = query.trimmed();

    if (query.isEmpty()) {
        // it shouldn't get to this point but let's be precautious
        setSearchError(tr("Nothing to show"), tr("Your search query is empty"));

        return;
    }
}
