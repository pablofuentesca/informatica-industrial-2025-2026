#pragma once
#include "terrestre.h"

// Equipo 1 (Madrid):  Unicornio (Bellingham) — ranged, el mas rapido del bando luz
// Equipo 2 (Atleti):  Basilisco (Koke)       — ranged, su impacto paraliza al rival 1 s
class Lateral : public Terrestre {
public:
    Lateral(float x, float y, int equipo)
        : Terrestre(x, y, equipo, equipo == 1
            ? "../bin/imagenes/fotosjugadores/madridlateral.png"
            : "../bin/imagenes/fotosjugadores/atletilateral.png")
    {
        if (equipo == 1) {
            // Unicornio: rapido y agil, disparo de rayo magico
            hpMax       = 60;
            velArena    = 220.0;
            danio       = 15;
            cooldownMax = 0.60;
        } else {
            // Basilisco: lento pero su mirada petrificante congela al rival
            hpMax       = 55;
            velArena    = 150.0;
            danio       = 15;
            cooldownMax = 0.70;
        }
        hp = hpMax;
    }

    int    getRango()      const override { return 3; }
    bool   esRanged()      const override { return true; }
    double alcanceAtaque() const override { return 0.0; }

    // Unicornio: movimiento terrestre agil
    // Basilisco: movimiento lento y amenazante
    void mover(float dirX, float dirY) override {
        Jugador::mover(dirX, dirY);
    }

    // Basilisco (equipo 2): al impactar con su proyectil, la Arena aplica
    // un efecto de paralisis de 1 segundo sobre el rival golpeado
    void habilidadEspecial() override {}

    bool esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const override;
};
