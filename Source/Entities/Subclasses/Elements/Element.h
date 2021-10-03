#pragma once
#include "Entity.h"

class Element : public virtual Entity {
    public:
        GIVE_TYPE_ID_1(10, Entity)
        
        DELETE_ILLEGAL_CONSTRUCTORS(Element)
        explicit Element() {}
        virtual ~Element() {}

        virtual void Update() {}
};