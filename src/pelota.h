#pragma once
#include "Ente.h"

// Le decimos al compilador que existe una clase Sprite en ETSIDI
namespace ETSIDI { class Sprite; }

class Pelota : public Ente {

    float radio;

    // Nuestro sprite que dibujará la imagen
    ETSIDI::Sprite* sprite;

public:
    Pelota(float x, float y);
    ~Pelota();

    void dibuja();
    void mueve(float t);
};