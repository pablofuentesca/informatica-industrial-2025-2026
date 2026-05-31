#pragma once
#include "terrestre.h"

// Base de movimiento compartida — no instanciar directamente
class Portero : public Terrestre {
protected:
    Portero(float x, float y, int eq, const char* ruta) : Terrestre(x, y, eq, ruta) {}
public:
    bool esRanged() const override { return true; }
    double alcanceAtaque() const override { return 0.0; }
    void mover(float dirX, float dirY) override { Jugador::mover(dirX, dirY); }
    void habilidadEspecial() override {}
    bool esMovimientoValido(int ox, int oy, int dx, int dy) const override = 0;
};

// Arquero — Madrid (equipo 1)
// Cadencia muy alta, vida baja; especialista en disparo a distancia
// Se mueve en diagonal hasta 3 casillas (Alfil)
class Arquero : public Portero {
public:
    Arquero(float x, float y): Portero(x, y, 1, "../bin/imagenes/fotosjugadores/madridportero.png")
    {
        hpMax = 30; velArena = 200.0; danio = 12; cooldownMax = 0.25;
        hp = hpMax;
    }
    int getRango() const override { return 3; }
    bool esMovimientoValido(int ox, int oy, int dx, int dy) const override;
};

// Manticora — Atleti (equipo 2)
// Resistente; lanza un abanico de 3 proyectiles simultaneos
// Se mueve 2 o 3 pasos en cualquier direccion
class Manticora : public Portero {
public:
    Manticora(float x, float y): Portero(x, y, 2, "../bin/imagenes/fotosjugadores/atletiportero.png")
    {
        hpMax = 70; velArena = 180.0; danio = 18; cooldownMax = 0.80;
        disparaTriple = true;
        hp = hpMax;
    }
    int  getRango() const override { return 3; }
    bool esMovimientoValido(int ox, int oy, int dx, int dy) const override;
};
