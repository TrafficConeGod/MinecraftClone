#pragma once
#include "Entity.h"
#include "ChunkGraphicsNode.h"
#include "Block.h"

static constexpr uint BlockHandlers = 2;

class BlockHandler : public virtual Entity {
    public:
        GIVE_TYPE_ID_1(13, Entity)

        DELETE_ILLEGAL_CONSTRUCTORS(BlockHandler)
        explicit BlockHandler() {}
        virtual ~BlockHandler() {}

        virtual bool IsTransparent(const Block& block) const;
        virtual void GenerateFaceMesh(ChunkGraphicsNode::Mesh& mesh, const Vector3u& position, const Block& block, Block::Face face) const {}
};