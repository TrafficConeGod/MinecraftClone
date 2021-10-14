#include "GrassBlockHandler.h"

Block::TextureId GrassBlockHandler::TextureIdFor(Block::Face face) const {
    switch (face) {
        case Block::Face::Top: return 0;
        case Block::Face::Bottom: return 2;
        case Block::Face::Front:
        case Block::Face::Back:
        case Block::Face::Right:
        case Block::Face::Left: return 3;
        default: return 0;
    }
    return 0;
}