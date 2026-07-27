#pragma once

#include "../common/errorinfo.h"
#include "station.h"
#include <QRestAccessManager>
#include <QRestReply>

namespace SourceConstraints {
const int DefaultStationsCacheExpiry = 60 * 10; // 10 minutes
}

class Source : public QObject {
    Q_OBJECT

protected:
    QNetworkAccessManager *m_networkAccessManager = nullptr;
    QRestAccessManager *m_restAccessManager = nullptr;

    void raiseError(const QString &title, const QString &message);

    bool parseJson(QRestReply &reply, QJsonDocument *json);

    void cacheDefaultStations(const QList<Station> &stations);
    const QList<Station> &getCachedDefaultStations();

    virtual void handleSearch(const QString &query) = 0;

    virtual void handleLoadDefaultStations();

private:
    struct DefaultStations {
    private:
        QList<Station> m_stations;
        qint64 m_cachedAt = -1;

        void resetCache();

    public:
        void setCached(const QList<Station> &stations);
        const QList<Station> &getCached();
    };

    DefaultStations m_defaultStations;

public:
    explicit Source();

    void search(const QString &query);
    virtual void cancelSearch() = 0;

    virtual bool hasDefaultStations() const;
    void loadDefaultStations();

    virtual QString websiteUrl() const = 0;

signals:
    void stationsDispatched(const QList<Station> &stations);
    void errorOccurred(const ErrorInfo &error);

    void searchStarted();
    void searchCancelled();
};
