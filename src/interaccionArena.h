#pragma once
class Arena;

class interaccionArena {
public:
    static void separa(Arena& arena);
    static void aplicaDanio(Arena& arena, double dt);
};
