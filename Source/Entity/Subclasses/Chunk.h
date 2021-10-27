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
        using Seed = uint;
        using Mesh = ChunkGraphicsNode::Mesh;
        using IsBlockAtWorldPositionTransparent = std::function<bool(const Vector3i&, const Block&)>;
        
        static constexpr int Bounds = 16;
        static constexpr float OffsetToCenter = 8.5;
        static constexpr std::size_t Blocks = 4096;
    private:
        const IsBlockAtWorldPositionTransparent& isBlockAtWorldPositionTransparent;
        const std::array<EntityReference<BlockHandler>, Block::Types>& blockHandlers;
        EntityReference<ChunkGraphicsNode> node;
        Vector3i position;
        std::array<Block, Blocks> blocks;
        std::vector<EntityReference<BlockEntity>> blockEntities;

        void GenerateMesh(Mesh& mesh);

        bool GenerateFaceMesh(const Vector3i& direction, Block::Face face, const EntityReference<BlockHandler> blockHandler, Mesh& chunkMesh, const Vector3u& position, const Block& block);


        static constexpr std::size_t MaxGenerationHeight = 32;
    public:
        GIVE_TYPE_ID_1(6, Entity)

        DELETE_ILLEGAL_CONSTRUCTORS(Chunk)
        explicit Chunk(const IsBlockAtWorldPositionTransparent& isBlockAtWorldPositionTransparent, const std::array<EntityReference<BlockHandler>, Block::Types>& blockHandlers, EntityReference<ChunkGraphicsNode> node, const Vector3i& position);
        explicit Chunk(const IsBlockAtWorldPositionTransparent& isBlockAtWorldPositionTransparent, const std::array<EntityReference<BlockHandler>, Block::Types>& blockHandlers, EntityReference<ChunkGraphicsNode> node, const Vector3i& position, const std::array<Block, Blocks>& blocks);
        virtual ~Chunk() {}

        void GenerateBlocks(Seed seed);

        const Block& BlockAt(const Vector3u& position) const;
        const Block& BlockAt(std::size_t index) const;
        bool IsBlockAtLocalPositionTransparent(const Vector3u& position, const Block& neighborBlock) const;
        Vector3i Position() const;
        void BlockAt(const Vector3u& position, const Block& block);
        void MakeMeshGenerate();

        void UpdateMesh();
        void Update();

        static Vector3i FreePositionToGridPosition(const Vector3f& freePosition);
        static Vector3i LocalChunkPositionToWorldPosition(const Vector3i& chunkPosition, const Vector3u& localPosition);
        static Vector3i WorldPositionToChunkPosition(const Vector3f& worldPosition);
        static Vector3u WorldPositionToLocalChunkPosition(const Vector3f& worldPosition);
        static std::size_t PositionToIndex(const Vector3i& position);
        static Vector3u IndexToPosition(std::size_t index);
};