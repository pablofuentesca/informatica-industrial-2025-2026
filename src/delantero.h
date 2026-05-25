#pragma once
#include "terrestre.h"

// Equipo 1 (Madrid):  Caballero (Mbappe)     — mele, el combatiente mas letal, vida maxima
// Equipo 2 (Atleti):  Goblin    (infanteria) — mele, debil individualmente, abundante
class Delantero : public Terrestre {
public:
    Delantero(float x, float y, int equipo)
        : Terrestre(x, y, equipo, equipo == 1
            ? "../bin/imagenes/fotosjugadores/madriddelantero.png"
            : "../bin/imagenes/fotosjugadores/atletidelantero.png")
    {
        if (equipo == 1) {
            // Caballero: el mas resistente y poderoso en combate cuerpo a cuerpo
            hpMax       = 100;
            velArena    = 180.0;
            danio       = 25;
            cooldownMax = 0.70;
        } else {
            // Goblin: rapido y agresivo pero muy fragil
            hpMax       = 25;
            velArena    = 210.0;
            danio       = 8;
            cooldownMax = 0.50;
        }
        hp = hpMax;
    }

    int    getRango()      const override { return 3; }
    bool   esRanged()      const override { return false; }

    // Hitbox de ataque: el cuerpo mas el alcance del arma (espada/lanza)
    // Caballero tiene mas alcance que el Goblin
    double alcanceAtaque() const override {
        return (equipo == 1) ? 40.0 : 20.0;
    }

    // Movimiento terrestre directo; el Caballero embiste al contactar
    void mover(float dirX, float dirY) override {
        Jugador::mover(dirX, dirY);
    }

    bool esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const override;
};
