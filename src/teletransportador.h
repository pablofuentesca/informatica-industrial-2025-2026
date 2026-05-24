#pragma once
#include "jugador.h"

class Teletransportador : public Jugador {
public:
    Teletransportador(float x, float y, int equipo, const char* ruta)
        : Jugador(x, y, equipo, ruta) {}
    bool esVolador()  const override { return false; }
    bool esTeleport() const override { return true;  }
};
