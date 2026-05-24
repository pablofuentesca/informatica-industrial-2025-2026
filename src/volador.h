#pragma once
#include "jugador.h"

class Volador : public Jugador {
public:
    Volador(float x, float y, int equipo, const char* ruta)
        : Jugador(x, y, equipo, ruta) {}
    bool esVolador()  const override { return true;  }
    bool esTeleport() const override { return false; }
};
