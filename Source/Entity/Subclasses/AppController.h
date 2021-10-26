#pragma once
#include "Entity.h"
#include "EntityReference.h"
#include "Thread.h"

class AppController : public virtual Entity {
    private:
        std::vector<EntityReference<Thread>> workers;
    public:
        GIVE_TYPE_ID_1(2, Entity)

        DELETE_ILLEGAL_CONSTRUCTORS(AppController)
        explicit AppController();
        virtual ~AppController() {}
};