#pragma once
#include "TexturedCubeBlockHandler.h"

class WaterBlockHandler : public virtual TexturedCubeBlockHandler {
    protected:
        virtual Vector2u TexturePositionFor(Block::Face face) const override;
    public:
        GIVE_TYPE_ID_1(19, TexturedCubeBlockHandler)

        DELETE_ILLEGAL_CONSTRUCTORS(WaterBlockHandler)
        explicit WaterBlockHandler() {}
        virtual ~WaterBlockHandler() {}

        virtual bool IsTransparent(const Block& block, const Block& neighborBlock) const override;
};