#pragma once

#include "../player/station.h"
#include "sourceerror.h"

class Source : public QObject {
    Q_OBJECT

protected:
    void raiseError(const QString &title, const QString &message);

public:
    explicit Source(QObject *parent = nullptr);

    virtual void search(const QString &query) = 0;
    virtual void cancelSearch() = 0;

    virtual bool hasDefaultStations() const;
    virtual void loadDefaultStations();

signals:
    void stationsDispatched(const QList<Station> &stations);
    void errorOccurred(const SourceError &error);

    void searchStarted();
    void searchCancelled();
};
