#include "GrassBlockHandler.h"

Vector2u GrassBlockHandler::TexturePositionFor(Block::Face face) const {
    switch (face) {
        case Block::Face::Top: return Vector2u(0, 0);
        case Block::Face::Bottom: return Vector2u(2, 0);
        case Block::Face::Front:
        case Block::Face::Back:
        case Block::Face::Right:
        case Block::Face::Left: return Vector2u(3, 0);
        default: break;
    }
    return Vector2u(0, 0);
}