#pragma once
#include "posicion.h"

namespace ETSIDI { class Sprite; }

class Pelota {
    float radio;
    ETSIDI::Sprite* sprite;

public:
    Posicion pos; // Le damos su propia posición
    Pelota(float x, float y);
    ~Pelota();

    void dibuja();
    void mueve(float t);
};