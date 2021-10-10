#pragma once
#include "TexturedCubeBlockHandler.h"

class GrassBlockHandler : public virtual TexturedCubeBlockHandler {
    protected:
        virtual uint TextureIdFor(Block::Face face) const override;
    public:
        GIVE_TYPE_ID_1(17, TexturedCubeBlockHandler)

        DELETE_ILLEGAL_CONSTRUCTORS(GrassBlockHandler)
        explicit GrassBlockHandler() {}
        virtual ~GrassBlockHandler() {}
};