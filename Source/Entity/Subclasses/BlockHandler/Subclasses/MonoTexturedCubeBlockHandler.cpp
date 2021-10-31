#include "MonoTexturedCubeBlockHandler.h"

MonoTexturedCubeBlockHandler::MonoTexturedCubeBlockHandler(Vector2u vTexturePosition) : texturePosition{vTexturePosition} {}

Vector2u MonoTexturedCubeBlockHandler::TexturePositionFor(Block::Face) const {
    return texturePosition;
}