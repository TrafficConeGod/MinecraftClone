#pragma once
#include "Entity.h"
#include "EntityReference.h"
#include "ChunkGraphicsNode.h"
#include "BlockHandler.h"
#include "Vector3.h"
#include "BlockEntity.h"
#include <array>
#include <map>

class Chunk : public virtual Entity {
    public:
        using Mesh = ChunkGraphicsNode::Mesh;
        
        static constexpr int Bounds = 16;
        static constexpr float OffsetToCenter = 8.5;
        static constexpr std::size_t Blocks = 4096;
    private:
        EntityReference<ChunkGraphicsNode> node;
        Vector3i position;
        std::array<Block, Blocks> blocks;
        std::vector<EntityReference<BlockEntity>> blockEntities;
        const std::array<EntityReference<BlockHandler>, Block::Types>& blockHandlers;

        void GenerateMesh(Mesh& mesh);

        EntityReference<BlockHandler> BlockHandlerFor(Block::Type type);

        bool GenerateFaceMesh(const Vector3i& direction, Block::Face face, const EntityReference<BlockHandler> blockHandler, Mesh& chunkMesh, const Vector3u& position, const Block& block);
    public:
        GIVE_TYPE_ID_1(6, Entity)

        DELETE_ILLEGAL_CONSTRUCTORS(Chunk)
        explicit Chunk(const Vector3i& position, const std::array<Block, Blocks>& blocks, const std::array<EntityReference<BlockHandler>, Block::Types>& blockHandlers, EntityReference<ChunkGraphicsNode> node);
        virtual ~Chunk() {}

        void SetBlock(const Vector3u& position, const Block& block);

        void Update();

        static Vector3i ChunkPositionToWorldPosition(const Vector3i& chunkPosition, const Vector3i& localPosition);
        static std::size_t PositionToIndex(const Vector3i& position);
        static Vector3u IndexToPosition(std::size_t index);
};