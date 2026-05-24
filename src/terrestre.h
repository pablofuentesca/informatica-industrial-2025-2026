#pragma once
#include "jugador.h"

class Terrestre : public Jugador {
public:
    Terrestre(float x, float y, int equipo, const char* ruta)
        : Jugador(x, y, equipo, ruta) {}
    bool esVolador()  const override { return false; }
    bool esTeleport() const override { return false; }
};
