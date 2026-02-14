#include "station.h"

QString Station::qUrlParse(const QString &rawUrl) const {
    return QUrl::fromUserInput(rawUrl).toString();
}

void Station::setValid(bool newValid) {
    m_valid = newValid;
}

Station::Station(const QString &name, const QString &streamUrl,
                 const QString &imageUrl) {
    setName(name);
    setStreamUrl(streamUrl);
    setImageUrl(imageUrl);
}

bool Station::isValid() const {
    return m_valid;
}

QString Station::name() const {
    return m_name;
}

void Station::setName(QString newName) {
    newName = newName.trimmed();

    if (newName.isEmpty()) {
        newName = QObject::tr("Unnamed Station");
    }

    m_name = newName;
}

QString Station::streamUrl() const {
    return m_streamUrl;
}

void Station::setStreamUrl(QString newStreamUrl) {
    m_streamUrl = qUrlParse(newStreamUrl);

    setValid(!m_streamUrl.isEmpty());
}

QString Station::imageUrl() const {
    return m_imageUrl;
}

void Station::setImageUrl(QString newImageUrl) {
    m_imageUrl = qUrlParse(newImageUrl);
}

bool Station::operator==(const Station &other) const {
    return (this->name() == other.name() &&
            this->streamUrl() == other.streamUrl() &&
            this->imageUrl() == other.imageUrl());
}
