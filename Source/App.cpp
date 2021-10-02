#include "Entity.h"
#include "EntityReference.h"
#include <iostream>

class MyEntity : public virtual Entity {
    private:
        int value;
    public:
        static constexpr Entity::TypeId Type = 1;

        MyEntity(int vValue) : Entity(), value{vValue} {}
        virtual ~MyEntity() {}

        static constexpr bool IsOfType(Entity::TypeId type) {
            return type == Type || Entity::IsOfType(type);
        }
};

int main() {
    EntityReference<Entity> ent = new Entity();
    EntityReference<MyEntity> myEnt = ent;

    return 0;
}