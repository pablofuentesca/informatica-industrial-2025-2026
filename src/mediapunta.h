#pragma once
#include "volador.h"

// Equipo 1 (Madrid):  Genio        (Valverde) — ranged, dispara desde cualquier zona
// Equipo 2 (Atleti):  Cambiaformas (Correa)   — ranged, copia ataque y habilidad del rival
class Mediapunta : public Volador {
public:
    Mediapunta(float x, float y, int equipo)
        : Volador(x, y, equipo, equipo == 1
            ? "../bin/imagenes/fotosjugadores/madridmediapunta.png"
            : "../bin/imagenes/fotosjugadores/atletimediapunta.png")
    {
        hpMax       = 70;
        velArena    = 200.0;
        danio       = 15;
        cooldownMax = 0.80;
        hp = hpMax;
    }

    // Genio (Madrid):    rango 4 — puede moverse a cualquier casilla libre (tablero)
    // Cambiaformas (At): rango 5 — se acerca al rival para copiarlo bien
    int getRango() const override { return equipo == 1 ? 4 : 5; }

    bool   esRanged()      const override { return true; }
    double alcanceAtaque() const override { return 0.0; }

    void mover(float dirX, float dirY) override {
        Jugador::mover(dirX, dirY);
    }

    // Cambiaformas (equipo 2): al inicio del combate copia el ataque del rival
    void habilidadEspecial() override {}

    bool esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const override;
};
