#pragma once
#include "TexturedCubeBlockHandler.h"

class MonoTexturedCubeBlockHandler : public virtual TexturedCubeBlockHandler {
    private:
        Block::TextureId textureId;
    protected:
        virtual Block::TextureId TextureIdFor(Block::Face) const override;
    public:
        GIVE_TYPE_ID_1(16, TexturedCubeBlockHandler)

        DELETE_ILLEGAL_CONSTRUCTORS(MonoTexturedCubeBlockHandler)
        explicit MonoTexturedCubeBlockHandler(Block::TextureId textureId);
        virtual ~MonoTexturedCubeBlockHandler() {}
};