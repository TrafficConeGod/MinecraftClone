#pragma once
#include "Element.h"
#include "Vector3.h"
#include "Event.h"

class Camera : public virtual Element {
    public:
        using Updated = Event<const Vector3f&>;
    private:
        Updated updated;

        Vector3f position;

        void Position(const Vector3f& position);
    public:
        GIVE_TYPE_ID_1(11, Element)
        
        DELETE_ILLEGAL_CONSTRUCTORS(Camera)
        explicit Camera(const Updated& updated = Updated());
        virtual ~Camera() {}
        
};