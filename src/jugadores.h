#pragma once
#include "posicion.h"

class Jugadores {
public:
    Posicion pos;

    Jugadores();
    ~Jugadores();

    void dibuja();
    void mueve(float t);
}; 
