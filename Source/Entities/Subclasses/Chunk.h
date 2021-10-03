#pragma once
#include "Entity.h"
#include "EntityReference.h"
#include "GraphicsNode.h"
#include "Vector3.h"
#include "BlockEntity.h"
#include <array>
#include <map>

class Chunk : public virtual Entity {
    public:
        struct Block {
            enum class Type {
                Air,
                Grass,
                Stone
            };
            Type type;
        };
        
        static constexpr std::size_t BlocksSize = 4096;
    private:
        std::array<Block, BlocksSize> blocks;
        std::vector<EntityReference<BlockEntity>> blockEntities;
        EntityReference<GraphicsNode> node;
    public:
        GIVE_TYPE_ID_1(6, Entity)

        DELETE_ILLEGAL_CONSTRUCTORS(Chunk)
        explicit Chunk(const std::array<Block, 4096>& blocks, EntityReference<GraphicsNode> node);
        virtual ~Chunk() {}

        void SetBlock(Vector2u location);

        void Update();
};