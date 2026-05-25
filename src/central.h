#pragma once
#include "terrestre.h"

class Central : public Terrestre {
public:
    Central(float x, float y, int equipo)
        : Terrestre(x, y, equipo, equipo == 1
            ? "../bin/imagenes/fotosjugadores/madriddefensa.png"
            : "../bin/imagenes/fotosjugadores/atletidefensa.png") {}

    int getRango() const override { return 3; }
    bool esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const override;
};
