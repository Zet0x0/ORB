#pragma once

#include <QHash>
#include <QObject>
#include <QString>
#include <cstdint>
#include <functional>
#include <optional>

struct mpv_handle;
struct mpv_event;

class Mpv : public QObject {
    Q_OBJECT

public:
    // error is empty if the request succeeded
    using Done = std::function<void(const QString &error)>;

    explicit Mpv(QObject *parent = nullptr);
    ~Mpv() override;

    QString version() const;

    void loadFile(const QString &url, Done done);
    void stop(Done done);

    void setVolume(int volume, Done done);
    void setMuted(bool muted, Done done);

    void shutdown();

signals:
    void fileStarted();
    void fileLoaded();
    void fileEnded(bool failed, const QString &reason);

    // filtered media-title
    void titleChanged(const QString &title);
    void timePosChanged(double seconds);

private:
    using Reply = std::function<void(const mpv_event &reply)>;

    mpv_handle *m_handle = nullptr;
    QString m_version;

    uint64_t m_nextReplyId = 1;
    QHash<uint64_t, Reply> m_replies;

    std::optional<QString> m_pendingTitle;

    void readVersion();

    void request(const std::function<int(uint64_t id)> &send, Reply onReply);

    void readEvents();

    void onPropertyChange(const mpv_event &event);
    void onReply(const mpv_event &event);
    void onEndFile(const mpv_event &event);

    void resolveTitle(const QString &mediaTitle);
};
