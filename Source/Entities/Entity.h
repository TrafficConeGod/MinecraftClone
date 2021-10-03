#pragma once
#include <cstdlib>
#include <iostream>

#define GIVE_TYPE_ID_0(id) \
static constexpr TypeId Type = id; \
static constexpr bool IsOfType(TypeId type) { return type == Type; }

#define GIVE_TYPE_ID_1(id, parent) \
static constexpr TypeId Type = id; \
static constexpr bool IsOfType(TypeId type) { return type == Type || parent::IsOfType(type); }

#define DELETE_ILLEGAL_CONSTRUCTORS(T) \
T(const T&) = delete; \
void operator=(const T&) = delete; \
T(T&&) = delete; \

class Entity {
    public:
        using TypeId = ushort;

        GIVE_TYPE_ID_0(0)
        
        DELETE_ILLEGAL_CONSTRUCTORS(Entity)
        explicit Entity() {}
        virtual ~Entity() {}
};