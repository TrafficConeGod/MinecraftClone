#pragma once
#include "Element.h"
#include "Vector3.h"
#include <functional>

class Camera : public virtual Element {
    public:
        using Update = std::function<void(const Vector3f&)>;
    private:
        Update update;

        Vector3f position;

        void Position(const Vector3f& position);
    public:
        GIVE_TYPE_ID_1(11, Element)
        
        DELETE_ILLEGAL_CONSTRUCTORS(Camera)
        explicit Camera(const Update& update);
        virtual ~Camera() {}
        
};