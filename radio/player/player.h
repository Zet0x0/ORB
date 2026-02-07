#pragma once

#include <MpvController>
#include <QQmlEngine>
#include <QThread>

class Player : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

private:
    MpvController *m_mpvController = nullptr;
    QThread *m_workerThread = nullptr;

public:
    explicit Player(QObject *parent = nullptr);
    ~Player();

    static Player *instance();
};
