#pragma once
#include "Thread.h"
#include "EntityReference.h"
#include "Camera.h"
#include "Element.h"
#include <functional>
#include <vector>

class UserInterfaceThread : public virtual Thread {
    private:
        UserInputEvents userInputEvents;

        Camera::CoordUpdate updateCamera;

        EntityReference<Camera> camera;
        std::vector<EntityReference<Element>> elements;
    protected:
        virtual void Update() override;
    public:
        GIVE_TYPE_ID_1(9, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(UserInterfaceThread)
        explicit UserInterfaceThread(const UserInputEvents& userInputEvents, const Camera::CoordUpdate& updateCamera);
        virtual ~UserInterfaceThread() {}
};