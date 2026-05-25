#pragma once
#include "terrestre.h"

class Portero : public Terrestre {
public:
    Portero(float x, float y, int equipo)
        : Terrestre(x, y, equipo, equipo == 1
            ? "../bin/imagenes/fotosjugadores/madridportero.png"
            : "../bin/imagenes/fotosjugadores/atletiportero.png") {}

    int getRango() const override { return 3; }
    bool esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const override;
};
