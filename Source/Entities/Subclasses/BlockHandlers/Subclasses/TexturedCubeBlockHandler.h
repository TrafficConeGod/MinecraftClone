#pragma once
#include "BlockHandler.h"

class TexturedCubeBlockHandler : public virtual BlockHandler {
    protected:
        virtual uint TextureIdFor(Block::Face face) const;
    public:
        GIVE_TYPE_ID_1(15, BlockHandler)

        DELETE_ILLEGAL_CONSTRUCTORS(TexturedCubeBlockHandler)
        explicit TexturedCubeBlockHandler() {}
        virtual ~TexturedCubeBlockHandler() {}

        virtual bool IsTransparent(const Block&, const Block&) const override;
        virtual void GenerateFaceMesh(ChunkGraphicsNode::Mesh& mesh, const Vector3u& position, const Block& block, Block::Face face) const override;
};