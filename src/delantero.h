#pragma once
#include "terrestre.h"

class Delantero : public Terrestre {
public:
    Delantero(float x, float y, int equipo)
        : Terrestre(x, y, equipo, equipo == 1
            ? "../bin/imagenes/fotosjugadores/madriddelantero.png"
            : "../bin/imagenes/fotosjugadores/atletidelantero.png") {}

    int getRango() const override { return 3; }
};
