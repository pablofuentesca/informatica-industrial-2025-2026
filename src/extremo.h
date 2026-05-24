#pragma once
#include "volador.h"

class Extremo : public Volador {
public:
    Extremo(float x, float y, int equipo)
        : Volador(x, y, equipo, equipo == 1
            ? "../bin/imagenes/fotosjugadores/madridextremo.png"
            : "../bin/imagenes/fotosjugadores/atletiextremo.png") {}

    // Fenix(Madrid)=5, Dragon(Atleti)=4
    int getRango() const override { return equipo == 1 ? 5 : 4; }
};
