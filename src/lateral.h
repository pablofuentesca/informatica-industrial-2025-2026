#pragma once
#include "terrestre.h"

// Base de movimiento compartida — no instanciar directamente
class Lateral : public Terrestre {
protected:
    Lateral(float x, float y, int eq, const char* ruta) : Terrestre(x, y, eq, ruta) {}
public:
    bool   esRanged()      const override { return true; }
    double alcanceAtaque() const override { return 0.0; }
    void   mover(float dirX, float dirY) override { Jugador::mover(dirX, dirY); }
    void   habilidadEspecial() override {}
    bool   esMovimientoValido(int ox, int oy, int dx, int dy) const override = 0;
};

// Unicornio — Madrid (equipo 1)
// Rapido y agil, dispara rayos magicos
// Se mueve en cualquier direccion (ortogonal o diagonal) hasta 2 casillas
class Unicornio : public Lateral {
public:
    Unicornio(float x, float y)
        : Lateral(x, y, 1, "../bin/imagenes/fotosjugadores/madridlateral.png")
    {
        hpMax = 60; velArena = 220.0; danio = 15; cooldownMax = 0.60;
        hp = hpMax;
    }
    int  getRango() const override { return 2; }
    bool esMovimientoValido(int ox, int oy, int dx, int dy) const override;
};

// Basilisco — Atleti (equipo 2)
// Lento y amenazante; su mirada petrificante paraliza al rival 1 segundo
// Se mueve en diagonal hasta 4 casillas
class Basilisco : public Lateral {
public:
    Basilisco(float x, float y)
        : Lateral(x, y, 2, "../bin/imagenes/fotosjugadores/atletilateral.png")
    {
        hpMax = 55; velArena = 150.0; danio = 15; cooldownMax = 0.70;
        proyectilParaliza = true;
        hp = hpMax;
    }
    int  getRango() const override { return 4; }
    bool esMovimientoValido(int ox, int oy, int dx, int dy) const override;
};
