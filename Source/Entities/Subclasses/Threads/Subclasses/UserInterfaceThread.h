#pragma once
#include "Thread.h"
#include "EntityReference.h"
#include "Camera.h"
#include "Element.h"
#include "Event.h"
#include <vector>

class UserInterfaceThread : public virtual Thread {
    public:
        using CameraUpdated = Event<const Vector3f&>;
    private:
        CameraUpdated cameraUpdated;

        EntityReference<Camera> camera;
        std::vector<EntityReference<Element>> elements;
    public:
        GIVE_TYPE_ID_1(9, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(UserInterfaceThread)
        explicit UserInterfaceThread(CameraUpdated cameraUpdated = CameraUpdated());
        virtual ~UserInterfaceThread() {}

        virtual void Update() override;
};