#pragma once

#include <QObject>

class MpvProperties : public QObject {
    Q_OBJECT

public:
    static const QString NowPlaying;
    static const QString Elapsed;
};
