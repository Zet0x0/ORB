#include "sourceerror.h"

bool SourceError::operator==(const SourceError &other) const {
    return this->title == other.title && this->message == other.message;
}
