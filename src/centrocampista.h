#pragma once
#include "volador.h"

class Centrocampista : public Volador {
public:
    Centrocampista(float x, float y, int equipo)
        : Volador(x, y, equipo, equipo == 1
            ? "../bin/imagenes/fotosjugadores/madridcentro.png"
            : "../bin/imagenes/fotosjugadores/atleticentro.png") {}

    int getRango() const override { return 3; }
    bool esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const override;
};
