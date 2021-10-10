#include "MonoTexturedCubeBlockHandler.h"

MonoTexturedCubeBlockHandler::MonoTexturedCubeBlockHandler(uint vTextureId) : textureId{vTextureId} {}

uint MonoTexturedCubeBlockHandler::TextureIdFor(Block::Face) const {
    return textureId;
}