#include "errorinfo.h"

ErrorInfo::ErrorInfo(QString newTitle, QString newMessage) {
    newTitle = newTitle.trimmed();
    newMessage = newMessage.trimmed();

    title = newTitle.isEmpty() ? QObject::tr("Error") : newTitle;
    message = newMessage.isEmpty() ? QObject::tr("An unknown error occurred")
                                   : newMessage;
}

bool ErrorInfo::operator==(const ErrorInfo &other) const {
    return this->title == other.title && this->message == other.message;
}
