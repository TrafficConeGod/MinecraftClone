#pragma once
#include "Thread.h"
#include "EntityReference.h"
#include "Camera.h"
#include "Control.h"
#include "Element.h"
#include <functional>
#include <vector>

class UserInterfaceThread : public virtual Thread {
    private:
        UserInput userInput;

        Camera::CoordUpdate updateCamera;
        Control::MouseClick mouseClick;

        EntityReference<Camera> camera;
        EntityReference<Control> control;
        std::vector<EntityReference<Element>> elements;

        Vector3f cameraPosition;
        Vector3f cameraLookVector;

        void UpdateCamera(const Vector3f& position, const Vector3f& lookVector);
    protected:
        virtual void Update(float delta) override;
    public:
        GIVE_TYPE_ID_1(9, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(UserInterfaceThread)
        explicit UserInterfaceThread(const UserInput& userInput, const Camera::CoordUpdate& updateCamera, const Control::MouseClick& mouseClick);
        virtual ~UserInterfaceThread() {}
};