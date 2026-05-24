#pragma once
#include "terrestre.h"

class Portero : public Terrestre {
public:
    Portero(float x, float y, int equipo)
        : Terrestre(x, y, equipo, equipo == 1
            ? "../bin/imagenes/fotosjugadores/madridportero.png"
            : "../bin/imagenes/fotosjugadores/atletiportero.png") {}

    int getRango() const override { return 3; }
};
