#include "BlankBlockHandler.h"

bool BlankBlockHandler::HasMesh(const Block&) const {
    return false;
}

bool BlankBlockHandler::IsTransparent(const Block&, const Block&) const {
    return true;
}