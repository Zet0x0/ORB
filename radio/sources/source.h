#pragma once

#include "../player/station.h"

class Source : public QObject {
    Q_OBJECT

public:
    explicit Source(QObject *parent = nullptr);

    virtual void search(const QString &query) = 0;
    virtual void cancelSearch() = 0;

    virtual bool hasDefaultStations();
    virtual void loadDefaultStations();

signals:
    void stationsDispatched(const QList<Station> &stations);
    void errorOccurred(const QString &message);

    void searchStarted();
    void searchCancelled();
};
