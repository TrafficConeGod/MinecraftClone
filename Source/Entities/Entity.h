#pragma once
#include <cstdlib>
#include <iostream>

class Entity {
    public:
        using TypeId = ushort;
        static constexpr TypeId Type = 0;
    public:
        static constexpr bool IsOfType(TypeId type) { return type == Type; }

        virtual ~Entity() {}
};