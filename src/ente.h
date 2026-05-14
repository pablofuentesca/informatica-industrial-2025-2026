#pragma once
#include "posicion.h"

class Ente {
public:
    Posicion pos;

    Ente();
    ~Ente();

    void dibuja();
    void mueve(float t);
}; 
