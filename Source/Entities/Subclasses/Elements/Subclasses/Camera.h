#pragma once
#include "Element.h"
#include "Vector3.h"
#include <functional>

class Camera : public virtual Element {
    public:
        using CoordUpdate = std::function<void(const Vector3f&, const Vector3f&)>;
    private:
        CoordUpdate coordUpdate;

        Vector3f position;
        Vector3f lookVector;

        void Position(const Vector3f& position);
        void LookVector(const Vector3f& lookVector);

        Vector2i lastCursorPosition;
    public:
        GIVE_TYPE_ID_1(11, Element)
        
        DELETE_ILLEGAL_CONSTRUCTORS(Camera)
        explicit Camera(const CoordUpdate& coordUpdate, const Vector3f& position, const Vector3f& lookVector);
        virtual ~Camera() {}

        virtual void Update(const UserInput& userInput) override;
};