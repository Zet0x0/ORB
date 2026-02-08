#pragma once

#include <QQmlEngine>

template <typename T> class Singleton {
public:
    static T *instance() {
        static T *instance = new T;

        return instance;
    } // C++ access

    static T *create(QQmlEngine *, QJSEngine *) {
        return instance();
    } // QML access
};
