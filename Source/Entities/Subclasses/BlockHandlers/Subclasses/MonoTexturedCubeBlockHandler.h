#pragma once
#include "TexturedCubeBlockHandler.h"

class MonoTexturedCubeBlockHandler : public virtual TexturedCubeBlockHandler {
    private:
        uint textureId;
    protected:
        virtual uint TextureIdFor(Block::Face) const override;
    public:
        GIVE_TYPE_ID_1(16, TexturedCubeBlockHandler)

        DELETE_ILLEGAL_CONSTRUCTORS(MonoTexturedCubeBlockHandler)
        explicit MonoTexturedCubeBlockHandler(uint textureId);
        virtual ~MonoTexturedCubeBlockHandler() {}
};