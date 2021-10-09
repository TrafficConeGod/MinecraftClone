#pragma once
#include "BlockHandler.h"

class GenericBlockHandler : public virtual BlockHandler {
    private:
        Block::Type type;
    public:
        GIVE_TYPE_ID_1(15, BlockHandler)

        DELETE_ILLEGAL_CONSTRUCTORS(GenericBlockHandler)
        explicit GenericBlockHandler(Block::Type type);
        virtual ~GenericBlockHandler() {}

        virtual bool IsTransparent(const Block&, const Block&) const override;
        virtual void GenerateFaceMesh(ChunkGraphicsNode::Mesh& mesh, const Vector3u& position, const Block& block, Block::Face face) const override;
};