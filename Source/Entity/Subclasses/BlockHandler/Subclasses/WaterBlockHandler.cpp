#include "WaterBlockHandler.h"

Vector2u WaterBlockHandler::TexturePositionFor(Block::Face face) const {
    return Vector2u(13, 12);
}

bool WaterBlockHandler::IsTransparent(const Block& block, const Block& neighborBlock) const {
    return neighborBlock.type != Block::Type::Water;
}