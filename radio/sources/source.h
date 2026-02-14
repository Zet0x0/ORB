#pragma once

#include <QObject>

class Source : public QObject {
    Q_OBJECT

public:
    explicit Source(QObject *parent = nullptr);

    virtual void search(const QString &query) = 0;
    virtual void cancelSearch() = 0;

signals:
    void searchStarted();
    void searchCancelled();
    void searchSuccessful();
    void searchErrorOccurred(const QString &message);
};
