#pragma once

#include "../singleton.h"
#include "source.h"
#include <QQmlEngine>

struct SearchError {
    Q_GADGET
    QML_NAMED_ELEMENT(searchError)

    Q_PROPERTY(QString title MEMBER title FINAL)
    Q_PROPERTY(QString message MEMBER message FINAL)

public:
    QString title;
    QString message;
};

class SourceController : public QObject, public Singleton<SourceController> {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(SourceController::SearchState searchState READ searchState NOTIFY
                   searchStateChanged FINAL)
    Q_PROPERTY(SearchError searchError READ searchError NOTIFY
                   searchErrorChanged FINAL)

    friend class Singleton<SourceController>;

public:
    enum class SearchState { Idle, Searching, Error };
    Q_ENUM(SearchState)

private:
    QHash<QString, Source *> m_sources;
    QStringList m_sourcesInsertOrder;

    Source *m_currentSearchSource;
    SourceController::SearchState m_searchState;
    SearchError m_searchError;

    explicit SourceController(QObject *parent = nullptr);

    void setSearchState(const SourceController::SearchState &newSearchState);
    void setSearchError(const QString &title, const QString &message);

    void cancelSearch();

private slots:
    void onSearchStarted();
    void onSearchCancelled();
    void onSearchSuccessful();
    void onSearchErrorOccurred(const QString &message);

public:
    bool registerSource(const QString &name, Source *source);
    bool sourceExists(const QString &sourceName) const;
    Q_INVOKABLE QStringList getSources() const;

    SourceController::SearchState searchState() const;
    SearchError searchError() const;

public slots:
    void search(const QString &sourceName, QString query);

signals:
    void searchStateChanged();
    void searchErrorChanged();
};
