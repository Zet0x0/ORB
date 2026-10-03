#include "mpv.h"
#include "../common/logcategories.h"
#include <QByteArray>
#include <QLatin1StringView>
#include <QLoggingCategory>
#include <QMetaObject>
#include <clocale>
#include <cstring>
#include <mpv/client.h>
#include <utility>

namespace {
namespace MpvProperties {
constexpr const char *MediaTitle = "media-title";
constexpr const char *Filename = "filename";
constexpr const char *TimePos = "time-pos";
constexpr const char *Volume = "volume";
constexpr const char *Mute = "mute";
constexpr const char *Version = "mpv-version";
}

// raw mpv version reads something like "mpv v0.41.0-1017-g02a595ddc"
constexpr QLatin1StringView MpvVersionPrefix("mpv ");

QString errorString(int error) {
    return QString::fromUtf8(mpv_error_string(error));
}

// a reply handler that passes on the error, if any
std::function<void(const mpv_event &)> outcome(Mpv::Done done) {
    return [done = std::move(done)](const mpv_event &reply) {
        done(reply.error < 0 ? errorString(reply.error) : QString());
    };
}

QString propertyString(const mpv_event_property &property) {
    return property.format == MPV_FORMAT_STRING
             ? QString::fromUtf8(*static_cast<char **>(property.data))
             : QString();
}

double propertyDouble(const mpv_event_property &property) {
    return property.format == MPV_FORMAT_DOUBLE
             ? *static_cast<double *>(property.data)
             : 0;
}

// make or return a saved QLoggingCategory out of a mpv's module
// e.g. ffmpeg/demuxer -> mpv.ffmpeg.demuxer
const QLoggingCategory &mpvCategory(const char *module) {
    static QHash<QByteArray, const QLoggingCategory *> categories;

    const QLoggingCategory *&category = categories[QByteArray(module)];

    if (!category) {
        const QByteArray name = "mpv." + QByteArray(module).replace("/", ".");

        // debug messages are hidden by default because they're mpv's verbose
        // output, which gives out a line for every downloaded stream segment;
        // to show them put a rule like mpv.*.debug=true
        category = new QLoggingCategory(qstrdup(name.constData()), QtInfoMsg);
    }

    return *category;
}

QString mpvMessageText(const mpv_event_log_message &message) {
    QString text = QString::fromUtf8(message.text);

    // ends in a newline, sometimes with spaces before it
    while (!text.isEmpty() && text.back().isSpace()) {
        text.chop(1);
    }

    return text;
}

void logMpvMessage(const mpv_event_log_message &message) {
    const QLoggingCategory &category = mpvCategory(message.prefix);

    switch (message.log_level) {
    case MPV_LOG_LEVEL_FATAL:
    case MPV_LOG_LEVEL_ERROR:
        qCCritical(category).noquote() << mpvMessageText(message);

        break;

    case MPV_LOG_LEVEL_WARN:
        qCWarning(category).noquote() << mpvMessageText(message);

        break;

    case MPV_LOG_LEVEL_INFO:
        qCInfo(category).noquote() << mpvMessageText(message);

        break;

    default:
        qCDebug(category).noquote() << mpvMessageText(message);

        break;
    }
}
}

Mpv::Mpv(QObject *parent) : QObject(parent) {
    std::setlocale(LC_NUMERIC, "C");

    m_handle = mpv_create();

    if (!m_handle) {
        qCFatal(lcMpv) << "Failed to create mpv";
    }

    mpv_set_property_string(m_handle, "vid", "no");

    mpv_request_log_messages(m_handle, "v");

    mpv_set_wakeup_callback(
        m_handle,
        [](void *mpv) {
            QMetaObject::invokeMethod(static_cast<Mpv *>(mpv), &Mpv::readEvents,
                                      Qt::QueuedConnection);
        },
        this);

    const int error = mpv_initialize(m_handle);

    if (error < 0) {
        qCFatal(lcMpv) << "Failed to initialize mpv:" << errorString(error);
    }

    mpv_observe_property(m_handle, 0, MpvProperties::MediaTitle,
                         MPV_FORMAT_STRING);
    mpv_observe_property(m_handle, 0, MpvProperties::TimePos,
                         MPV_FORMAT_DOUBLE);

    qCDebug(lcMpv) << "mpv initialized";

    readVersion();
}

Mpv::~Mpv() {
    shutdown();
}

QString Mpv::version() const {
    return m_version;
}

void Mpv::loadFile(const QString &url, Done done) {
    const QByteArray utf8Url = url.toUtf8();
    const char *args[] = {"loadfile", utf8Url.constData(), nullptr};

    request(
        [&](uint64_t id) {
            return mpv_command_async(m_handle, id, args);
        },
        outcome(std::move(done)));
}

void Mpv::stop(Done done) {
    const char *args[] = {"stop", nullptr};

    request(
        [&](uint64_t id) {
            return mpv_command_async(m_handle, id, args);
        },
        outcome(std::move(done)));
}

void Mpv::setVolume(int volume, Done done) {
    double value = volume;

    request(
        [&](uint64_t id) {
            return mpv_set_property_async(m_handle, id, MpvProperties::Volume,
                                          MPV_FORMAT_DOUBLE, &value);
        },
        outcome(std::move(done)));
}

void Mpv::setMuted(bool muted, Done done) {
    int value = muted;

    request(
        [&](uint64_t id) {
            return mpv_set_property_async(m_handle, id, MpvProperties::Mute,
                                          MPV_FORMAT_FLAG, &value);
        },
        outcome(std::move(done)));
}

void Mpv::shutdown() {
    if (!m_handle) {
        return;
    }

    qCDebug(lcMpv) << "Shutting down mpv";

    // get the last log lines out while we still can
    readEvents();

    mpv_set_wakeup_callback(m_handle, nullptr, nullptr);
    mpv_terminate_destroy(std::exchange(m_handle, nullptr));

    m_replies.clear();
}

// synchronous, so the About dialog can get the version
void Mpv::readVersion() {
    char *version = nullptr;
    const int error = mpv_get_property(m_handle, MpvProperties::Version,
                                       MPV_FORMAT_STRING, &version);

    if (error < 0) {
        qCWarning(lcMpv) << "Failed to read" << MpvProperties::Version
                         << errorString(error);

        return;
    }

    m_version = QString::fromUtf8(version);
    mpv_free(version);

    if (m_version.startsWith(MpvVersionPrefix)) {
        m_version.remove(0, MpvVersionPrefix.size());
    }

    qCInfo(lcMpv).noquote() << "Using mpv" << m_version;
}

// send makes the async call with the reply ID it's given,
// onReply gets the reply event
void Mpv::request(const std::function<int(uint64_t id)> &send, Reply onReply) {
    if (!m_handle) {
        return;
    }

    const uint64_t id = m_nextReplyId++;
    const int error = send(id);

    if (error < 0) {
        // refused right away, so no reply event will ever come
        // make one up and delay it in a way
        mpv_event reply{};
        reply.error = error;

        QMetaObject::invokeMethod(
            this,
            [onReply, reply] {
                onReply(reply);
            },
            Qt::QueuedConnection);

        return;
    }

    m_replies.insert(id, std::move(onReply));
}

void Mpv::readEvents() {
    while (m_handle) {
        const mpv_event *event = mpv_wait_event(m_handle, 0);

        switch (event->event_id) {
        case MPV_EVENT_NONE:
            return;

        case MPV_EVENT_LOG_MESSAGE:
            logMpvMessage(*static_cast<mpv_event_log_message *>(event->data));

            break;

        case MPV_EVENT_PROPERTY_CHANGE:
            onPropertyChange(*event);

            break;

        case MPV_EVENT_GET_PROPERTY_REPLY:
        case MPV_EVENT_SET_PROPERTY_REPLY:
        case MPV_EVENT_COMMAND_REPLY:
            onReply(*event);

            break;

        case MPV_EVENT_START_FILE:
            emit fileStarted();

            break;

        case MPV_EVENT_FILE_LOADED:
            emit fileLoaded();

            break;

        case MPV_EVENT_END_FILE:
            onEndFile(*event);

            break;

        default:
            break;
        }
    }
}

void Mpv::onPropertyChange(const mpv_event &event) {
    const mpv_event_property &property =
        *static_cast<mpv_event_property *>(event.data);

    if (std::strcmp(property.name, MpvProperties::MediaTitle) == 0) {
        resolveTitle(propertyString(property));
    } else if (std::strcmp(property.name, MpvProperties::TimePos) == 0) {
        emit timePosChanged(propertyDouble(property));
    }
}

void Mpv::onReply(const mpv_event &event) {
    const Reply handler = m_replies.take(event.reply_userdata);

    if (handler) {
        handler(event);
    }
}

void Mpv::onEndFile(const mpv_event &event) {
    const mpv_event_end_file &endFile =
        *static_cast<mpv_event_end_file *>(event.data);

    switch (endFile.reason) {
    case MPV_END_FILE_REASON_EOF:
        emit fileEnded(true, tr("the stream ended"));

        break;

    case MPV_END_FILE_REASON_ERROR:
        emit fileEnded(true, errorString(endFile.error));

        break;

    case MPV_END_FILE_REASON_STOP:
        emit fileEnded(false, QString());

        break;

    default:
        break;
    }
}

void Mpv::resolveTitle(const QString &mediaTitle) {
    const bool alreadyResolving = m_pendingTitle.has_value();
    m_pendingTitle = mediaTitle;

    if (alreadyResolving) {
        return;
    }

    request(
        [this](uint64_t id) {
            return mpv_get_property_async(m_handle, id, MpvProperties::Filename,
                                          MPV_FORMAT_STRING);
        },
        [this](const mpv_event &reply) {
            const QString title =
                std::exchange(m_pendingTitle, std::nullopt).value_or(QString());
            const QString filename =
                reply.error < 0
                    ? QString()
                    : propertyString(
                          *static_cast<mpv_event_property *>(reply.data));

            emit titleChanged(title == filename ? QString() : title);
        });
}
