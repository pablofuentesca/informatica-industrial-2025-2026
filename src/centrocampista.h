#pragma once
#include "volador.h"

// Base de movimiento compartida — no instanciar directamente
class Centrocampista : public Volador {
protected:
    Centrocampista(float x, float y, int eq, const char* ruta) : Volador(x, y, eq, ruta) {}
public:
    int getRango() const override { return 3; }
    void mover(float dirX, float dirY) override { Jugador::mover(dirX, dirY); }
    void habilidadEspecial() override {}
    bool esMovimientoValido(int ox, int oy, int dx, int dy) const override = 0;
};

// Valkiria — Madrid (equipo 1)
// Voladora que ataca cuerpo a cuerpo con espada
// Se mueve en L (como caballo de ajedrez), saltando sobre otras piezas
class Valkiria : public Centrocampista {
public:
    Valkiria(float x, float y) : Centrocampista(x, y, 1, "../bin/imagenes/fotosjugadores/madridcentro.png")
    {
        hpMax = 70; velArena = 240.0; danio = 18; cooldownMax = 0.60;
        hp = hpMax;
    }
    bool esRanged() const override { return false; }
    double alcanceAtaque() const override { return 40.0; }
    bool esMovimientoValido(int ox, int oy, int dx, int dy) const override;
};

// Banshee — Atleti (equipo 2)
// Voladora cuyo alarido atraviesa todos los obstaculos del campo
// Se mueve en cualquier direccion hasta 3 casillas, volando sobre piezas
class Banshee : public Centrocampista {
public:
    Banshee(float x, float y): Centrocampista(x, y, 2, "../bin/imagenes/fotosjugadores/atleticentro.png")
    {
        hpMax = 65; velArena = 240.0; danio = 16; cooldownMax = 0.70;
        proyectilAtraviesa = true;
        hp = hpMax;
    }
    bool esRanged()const override { return true; }
    double alcanceAtaque() const override { return 0.0; }
    bool esMovimientoValido(int ox, int oy, int dx, int dy) const override;
};
