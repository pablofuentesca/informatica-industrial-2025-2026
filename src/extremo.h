#pragma once
#include "volador.h"

// Base de movimiento compartida — no instanciar directamente
class Extremo : public Volador {
protected:
    Extremo(float x, float y, int eq, const char* ruta) : Volador(x, y, eq, ruta) {}
public:
    bool esRanged()      const override { return true; }
    double alcanceAtaque() const override { return 0.0; }
    void mover(float dirX, float dirY) override { Jugador::mover(dirX, dirY); }
    void habilidadEspecial() override {}
    bool esMovimientoValido(int ox, int oy, int dx, int dy) const override = 0;
};

// Fenix — Madrid (equipo 1)
// Reina del tablero: cualquier direccion hasta 5 casillas
// Habilidad: renace una vez al 50 % HP cuando es derrotada en arena
class Fenix : public Extremo 
{
    bool revivioUsado{ false };
public:
    Fenix(float x, float y) : Extremo(x, y, 1, "../bin/imagenes/fotosjugadores/madridextremo.png")
    {
        hpMax = 55; velArena = 200.0; danio = 15; cooldownMax = 0.80;
        hp = hpMax;
    }
    int getRango() const override { return 5; }
    bool puedeRevivir() const override { return !revivioUsado; }
    void revive() override { hp = hpMax / 2; revivioUsado = true; }
    bool esMovimientoValido(int ox, int oy, int dx, int dy) const override;
};

// Dragon — Atleti (equipo 2)
// Torre del tablero: solo movimiento ortogonal hasta 3 casillas
// Habilidad: chorro de fuego continuo (un proyectil cada 50 ms)
class Dragon : public Extremo {
public:
    Dragon(float x, float y) : Extremo(x, y, 2, "../bin/imagenes/fotosjugadores/atletiextremo.png")
    {
        hpMax = 100; velArena = 130.0; danio = 20; cooldownMax = 0.05;
        hp = hpMax;
    }
    int getRango() const override { return 3; }
    bool esMovimientoValido(int ox, int oy, int dx, int dy) const override;
};
