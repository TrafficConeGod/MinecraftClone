#pragma once
#include "Entity.h"
#include "UserInput.h"
#include "Vector3.h"

class Element : public virtual Entity {
    public:
        GIVE_TYPE_ID_1(10, Entity)
        
        DELETE_ILLEGAL_CONSTRUCTORS(Element)
        explicit Element() {}
        virtual ~Element() {}

        struct UpdateInfo {
            const UserInput& userInput;
            float delta;
            const Vector3f& cameraPosition;
            const Vector3f& cameraLookVector;
        };
        virtual void Update(const UpdateInfo& updateInfo) {}
};