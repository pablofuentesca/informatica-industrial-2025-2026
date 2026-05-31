#pragma once
#include "volador.h"

// Base de movimiento compartida — no instanciar directamente
// Ambos se mueven como alfil de ajedrez hasta 4 casillas
class Mediapunta : public Volador {
protected:
    Mediapunta(float x, float y, int eq, const char* ruta) : Volador(x, y, eq, ruta) {}
public:
    int getRango() const override { return 4; }
    bool esRanged() const override { return true; }
    double alcanceAtaque() const override { return 0.0; }
    void mover(float dirX, float dirY) override { Jugador::mover(dirX, dirY); }
    void habilidadEspecial() override {}
    bool esMovimientoValido(int ox, int oy, int dx, int dy) const override;
};

// Genio — Madrid (equipo 1)
// Mago errante que controla el campo desde cualquier zona en diagonal
class Genio : public Mediapunta {
public:
    Genio(float x, float y) : Mediapunta(x, y, 1, "../bin/imagenes/fotosjugadores/madridmediapunta.png")
    {
        hpMax = 70; velArena = 200.0; danio = 15; cooldownMax = 0.80;
        hp = hpMax;
    }
};

// Cambiaformas — Atleti (equipo 2)
// Al inicio del combate en arena copia la habilidad especial del rival
class Cambiaformas : public Mediapunta {
public:
    Cambiaformas(float x, float y) : Mediapunta(x, y, 2, "../bin/imagenes/fotosjugadores/atletimediapunta.png")
    {
        hpMax = 70; velArena = 200.0; danio = 15; cooldownMax = 0.80;
        hp = hpMax;
    }
};
