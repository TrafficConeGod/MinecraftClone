#pragma once
#include <iostream>

struct Block {
    enum class Face : u_char {
        Top,
        Bottom,
        Front,
        Back,
        Left,
        Right,
    };

    enum class Type : u_char {
        Air,
        Grass,
        Stone
    };
    Type type = Type::Air;
};