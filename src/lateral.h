#pragma once
#include "terrestre.h"

class Lateral : public Terrestre {
public:
    Lateral(float x, float y, int equipo)
        : Terrestre(x, y, equipo, equipo == 1
            ? "../bin/imagenes/fotosjugadores/madridlateral.png"
            : "../bin/imagenes/fotosjugadores/atletilateral.png") {}

    int getRango() const override { return 3; }
};
