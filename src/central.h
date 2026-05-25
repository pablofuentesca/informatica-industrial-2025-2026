#pragma once
#include "terrestre.h"

// Equipo 1 (Madrid):  Golem (Militao/Alaba)  — mele lento, muy resistente
// Equipo 2 (Atleti):  Trol  (Savic)         — mele lento, regenera vida pasivamente
class Central : public Terrestre {
public:
    Central(float x, float y, int equipo)
        : Terrestre(x, y, equipo, equipo == 1
            ? "../bin/imagenes/fotosjugadores/madriddefensa.png"
            : "../bin/imagenes/fotosjugadores/atletidefensa.png")
    {
        if (equipo == 1) {
            // Golem: gran resistencia, golpe pesado, muy lento
            hpMax       = 75;
            velArena    = 130.0;
            danio       = 20;
            cooldownMax = 0.80;
        } else {
            // Trol: aun mas resistente, regenera 5 HP/s, golpe devastador
            hpMax       = 90;
            velArena    = 130.0;
            danio       = 22;
            cooldownMax = 0.90;
        }
        hp = hpMax;
    }

    int    getRango()      const override { return 3; }
    bool   esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const override;
    bool   esRanged()      const override { return false; }

    // Hitbox de maza: mayor que la espada del Goblin, similar al Caballero
    double alcanceAtaque() const override { return 35.0; }

    // Movimiento lento y pesado sobre el terreno
    void mover(float dirX, float dirY) override {
        Jugador::mover(dirX, dirY);
    }

    // Trol (equipo 2): regenera vida en cada frame de la arena
    void actualiza(double dt) override {
        Jugador::actualiza(dt);
        if (equipo == 2 && hp < hpMax) {
            hp += static_cast<int>(5.0 * dt);
            if (hp > hpMax) hp = hpMax;
        }
    }
};
