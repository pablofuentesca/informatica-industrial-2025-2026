#pragma once
#include "terrestre.h"

// Equipo 1 (Madrid):  Arquero  (Rodrygo)  — ranged, cadencia alta, vida baja
// Equipo 2 (Atleti):  Manticora (Morata)  — ranged, disparo triple en abanico, mas resistente
class Portero : public Terrestre {
public:
    Portero(float x, float y, int equipo)
        : Terrestre(x, y, equipo, equipo == 1
            ? "../bin/imagenes/fotosjugadores/madridportero.png"
            : "../bin/imagenes/fotosjugadores/atletiportero.png")
    {
        if (equipo == 1) {
            hpMax       = 30;
            velArena    = 200.0;
            danio       = 12;
            cooldownMax = 0.25;
        } else {
            hpMax        = 70;
            velArena     = 180.0;
            danio        = 18;
            cooldownMax  = 0.80;
            disparaTriple = true;   // Manticora: abanico de 3 proyectiles
        }
        hp = hpMax;
    }

    int    getRango()      const override { return 3; }
    bool   esRanged()      const override { return true; }
    double alcanceAtaque() const override { return 0.0; }

    void mover(float dirX, float dirY) override {
        Jugador::mover(dirX, dirY);
    }

    // Manticora (equipo 2): dispara 3 proyectiles en abanico simultaneamente
    void habilidadEspecial() override {}

    bool esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const override;
};
