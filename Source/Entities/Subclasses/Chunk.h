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
            Type type = Type::Air;
        };
        
        struct BlockMesh {
            bool hasIndex = false;
            std::size_t index = 0;
        };
        
        static constexpr std::size_t Bounds = 16;
        static constexpr float OffsetToCenter = 8.5;
        static constexpr std::size_t Blocks = 4096;
    private:
        Vector3i position;
        std::array<Block, Blocks> blocks;
        std::array<BlockMesh, Blocks> blockMeshes;
        std::vector<EntityReference<BlockEntity>> blockEntities;
        EntityReference<GraphicsNode> node;
    public:
        GIVE_TYPE_ID_1(6, Entity)

        DELETE_ILLEGAL_CONSTRUCTORS(Chunk)
        explicit Chunk(const Vector3i& position, const std::array<Block, Blocks>& blocks, EntityReference<GraphicsNode> node);
        virtual ~Chunk() {}

        void SetBlock(std::size_t index, const Block& block);
        void SetBlock(const Vector3u& position, const Block& block);

        void Update();

        static std::size_t PositionToIndex(const Vector3u& position);
        static Vector3u IndexToPosition(std::size_t index);
};