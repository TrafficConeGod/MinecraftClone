#include "MonoTexturedCubeBlockHandler.h"

MonoTexturedCubeBlockHandler::MonoTexturedCubeBlockHandler(Block::TextureId vTextureId) : textureId{vTextureId} {}

Block::TextureId MonoTexturedCubeBlockHandler::TextureIdFor(Block::Face) const {
    return textureId;
}