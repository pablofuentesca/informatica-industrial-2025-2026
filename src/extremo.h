#pragma once
#include "volador.h"

// Equipo 1 (Madrid):  Fenix  (Carvajal) — ranged, si muere renace 1 vez con 50% HP
// Equipo 2 (Atleti):  Dragon (Griezmann)— ranged, disparo casi continuo (chorro de fuego)
class Extremo : public Volador {
    bool revivioUsado{ false };   // solo para el Fenix (equipo 1)

public:
    Extremo(float x, float y, int equipo)
        : Volador(x, y, equipo, equipo == 1
            ? "../bin/imagenes/fotosjugadores/madridextremo.png"
            : "../bin/imagenes/fotosjugadores/atletiextremo.png")
    {
        if (equipo == 1) {
            hpMax       = 55;
            velArena    = 200.0;
            danio       = 15;
            cooldownMax = 0.80;
        } else {
            hpMax       = 100;
            velArena    = 130.0;
            danio       = 20;
            cooldownMax = 0.05;   // chorro de fuego: un proyectil cada 50 ms
        }
        hp = hpMax;
    }

    // Fenix (Madrid): rango 5 — largo desplazamiento en tablero
    // Dragon (Atleti): rango 4 — mas corto, avanza lento
    int getRango() const override { return equipo == 1 ? 5 : 4; }

    bool   esRanged()      const override { return true; }
    double alcanceAtaque() const override { return 0.0; }

    void mover(float dirX, float dirY) override {
        Jugador::mover(dirX, dirY);
    }

    // Fenix (equipo 1): renace una vez con 50% HP
    bool puedeRevivir() const { return equipo == 1 && !revivioUsado; }

    void revive() {
        hp           = hpMax / 2;
        revivioUsado = true;
    }

    // Dragon (equipo 2): habilidad expresada en cooldownMax=0.05
    void habilidadEspecial() override {}

    bool esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const override;
};
