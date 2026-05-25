#pragma once
#include "volador.h"

class Mediapunta : public Volador {
public:
    Mediapunta(float x, float y, int equipo)
        : Volador(x, y, equipo, equipo == 1
            ? "../bin/imagenes/fotosjugadores/madridmediapunta.png"
            : "../bin/imagenes/fotosjugadores/atletimediapunta.png") {}

    // Genio(Madrid)=4, Cambiaformas(Atleti)=5
    int getRango() const override { return equipo == 1 ? 4 : 5; }
    bool esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const override;
};
