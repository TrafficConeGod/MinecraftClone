#pragma once
#include "Element.h"
#include "Vector3.h"

class Control : public virtual Element {
    public:
        using MouseClick = std::function<void(const Vector3f&, const Vector3f&)>;
    private:
        MouseClick mouseClick;
    public:
        GIVE_TYPE_ID_1(14, Element)
        
        DELETE_ILLEGAL_CONSTRUCTORS(Control)
        explicit Control(const MouseClick& mouseClick);
        virtual ~Control() {}

        virtual void Update(const UpdateInfo& updateInfo) override;
};