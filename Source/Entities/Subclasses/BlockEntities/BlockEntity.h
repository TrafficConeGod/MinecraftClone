#pragma once
#include "Entity.h"
#include "Vector3.h"

class BlockEntity : public virtual Entity {
    private:
        Vector3u location;
    public:
        GIVE_TYPE_ID_1(7, Entity)

        DELETE_ILLEGAL_CONSTRUCTORS(BlockEntity)
        explicit BlockEntity(const Vector3u& location);
        virtual ~BlockEntity() {}

        virtual void Update() {}
};