#pragma once
#include "TexturedCubeBlockHandler.h"

class MonoTexturedCubeBlockHandler : public virtual TexturedCubeBlockHandler {
    private:
        Vector2u texturePosition;
    protected:
        virtual Vector2u TexturePositionFor(Block::Face) const override;
    public:
        GIVE_TYPE_ID_1(16, TexturedCubeBlockHandler)

        DELETE_ILLEGAL_CONSTRUCTORS(MonoTexturedCubeBlockHandler)
        explicit MonoTexturedCubeBlockHandler(Vector2u texturePosition);
        virtual ~MonoTexturedCubeBlockHandler() {}
};