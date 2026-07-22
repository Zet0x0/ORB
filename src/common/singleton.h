#pragma once

#include <QQmlEngine>

template <typename T> class Singleton {
public:
    static T *instance() {
        static T *singleton = new T;

        return singleton;
    } // C++ access

    static T *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) {
        Q_UNUSED(qmlEngine)
        Q_UNUSED(jsEngine)

        return instance();
    } // QML access
};
