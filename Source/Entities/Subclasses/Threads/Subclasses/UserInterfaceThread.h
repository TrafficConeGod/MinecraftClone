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
        EntityReference<Camera> camera;
        
        CameraUpdated cameraUpdated;
    public:
        GIVE_TYPE_ID_1(9, Thread)

        DELETE_ILLEGAL_CONSTRUCTORS(UserInterfaceThread)
        explicit UserInterfaceThread(const CameraUpdated& cameraUpdated = CameraUpdated());
        virtual ~UserInterfaceThread() {}
};