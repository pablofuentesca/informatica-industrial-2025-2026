#pragma once
#include "teletransportador.h"

class Entrenador : public Teletransportador {
public:
    Entrenador(float x, float y, int equipo)
        : Teletransportador(x, y, equipo, equipo == 1
            ? "../bin/imagenes/fotosjugadores/madridentrenador.png"
            : "../bin/imagenes/fotosjugadores/atletientrenador.png") {}

    int getRango() const override { return 3; }
    bool esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const override;
};
