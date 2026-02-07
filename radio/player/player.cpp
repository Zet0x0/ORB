#include "player.h"

Player::Player(QObject *parent)
    : QObject(parent), m_workerThread(new QThread(this)) {
    m_mpvController = new MpvController;

    connect(m_workerThread, &QThread::finished, m_mpvController,
            &QObject::deleteLater);

    m_mpvController->moveToThread(m_workerThread);

    m_workerThread->start();

    QMetaObject::invokeMethod(m_mpvController, &MpvController::init,
                              Qt::BlockingQueuedConnection);
}

Player::~Player() {
    m_workerThread->quit();
    m_workerThread->wait();

    mpv_terminate_destroy(m_mpvController->mpv());

    m_workerThread->deleteLater();
}

Player *Player::instance() {
    static Player *instance = new Player;

    return instance;
}
