#pragma once
#include "terrestre.h"

// Base de movimiento compartida — no instanciar directamente
class Delantero : public Terrestre {
protected:
    Delantero(float x, float y, int eq, const char* ruta) : Terrestre(x, y, eq, ruta) {}
public:
    int getRango() const override { return 3; }
    void mover(float dirX, float dirY) override { Jugador::mover(dirX, dirY); }
    bool esMovimientoValido(int ox, int oy, int dx, int dy) const override = 0;
};

// Caballero — Madrid (equipo 1)
// El combatiente mas letal cuerpo a cuerpo; maximo HP del bando
// Se mueve en L (como caballo de ajedrez)
class Caballero : public Delantero {
public:
    Caballero(float x, float y) : Delantero(x, y, 1, "../bin/imagenes/fotosjugadores/madriddelantero.png")
    {
        hpMax = 100; velArena = 180.0; danio = 25; cooldownMax = 0.70;
        hp = hpMax;
    }
    bool esRanged()      const override { return false; }
    double alcanceAtaque() const override { return 50.0; }
    bool esMovimientoValido(int ox, int oy, int dx, int dy) const override;
};

// Goblin — Atleti (equipo 2)
// Rapido y numeroso; lanza un enjambre de 3 proyectiles en abanico de 30 grados
// Se mueve 1 paso en diagonal
class Goblin : public Delantero {
public:
    Goblin(float x, float y)
        : Delantero(x, y, 2, "../bin/imagenes/fotosjugadores/atletidelantero.png")
    {
        hpMax = 25; velArena = 210.0; danio = 8; cooldownMax = 0.50;
        disparaEnjambre = true;
        hp = hpMax;
    }
    bool esRanged() const override { return true; }
    double alcanceAtaque() const override { return 0.0; }
    bool esMovimientoValido(int ox, int oy, int dx, int dy) const override;
};
