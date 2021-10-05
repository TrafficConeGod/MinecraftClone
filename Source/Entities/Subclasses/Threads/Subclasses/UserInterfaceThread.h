#pragma once
#include "Thread.h"
#include "EntityReference.h"
#include "Camera.h"
#include "Element.h"
#include <functional>
#include <vector>

class UserInterfaceThread : public virtual Thread {
    private:
        UserInput userInput;

        Camera::CoordUpdate updateCamera;

        EntityReference<Camera> camera;
        std::vector<EntityReference<Element>> elements;
    protected:
        virtual void Update(float delta) override;
    public:
        GIVE_TYPE_ID_1(9, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(UserInterfaceThread)
        explicit UserInterfaceThread(const UserInput& userInput, const Camera::CoordUpdate& updateCamera);
        virtual ~UserInterfaceThread() {}
};