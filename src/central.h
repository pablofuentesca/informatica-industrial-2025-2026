#pragma once
#include "terrestre.h"

// Base de movimiento compartida — no instanciar directamente
// Ambos se mueven ortogonalmente (Torre)
class Central : public Terrestre {
protected:
    Central(float x, float y, int eq, const char* ruta) : Terrestre(x, y, eq, ruta) {}
public:
    bool esRanged()      const override { return false; }
    double alcanceAtaque() const override { return 45.0; }
    void mover(float dirX, float dirY) override { Jugador::mover(dirX, dirY); }
    bool esMovimientoValido(int ox, int oy, int dx, int dy) const override;
};

// Golem — Madrid (equipo 1)
// Gran resistencia y golpe pesado; se mueve ortogonalmente hasta 3 casillas
class Golem : public Central {
public:
    Golem(float x, float y) : Central(x, y, 1, "../bin/imagenes/fotosjugadores/madriddefensa.png")
    {
        hpMax = 75; velArena = 130.0; danio = 20; cooldownMax = 0.80;
        hp = hpMax;
    }
    int getRango() const override { return 3; }
};

// Trol — Atleti (equipo 2)
// Aun mas resistente que el Golem; regenera 5 HP por segundo en arena
// Se mueve ortogonalmente hasta 2 casillas
class Trol : public Central {
public:
    Trol(float x, float y)
        : Central(x, y, 2, "../bin/imagenes/fotosjugadores/atletidefensa.png")
    {
        hpMax = 90; velArena = 130.0; danio = 22; cooldownMax = 0.90;
        hp = hpMax;
    }
    int getRango() const override { return 2; }
    void actualiza(double dt) override {
        Jugador::actualiza(dt);
        if (hp < hpMax) {
            hp += static_cast<int>(5.0 * dt);
            if (hp > hpMax) hp = hpMax;
        }
    }
};
