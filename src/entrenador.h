#pragma once
#include "volador.h"
#include "hechicero.h"
class Mundo;

// Equipo 1 (Madrid):  Mago      (Ancelotti) — PIEZA REY, ranged, 7 conjuros por partida
// Equipo 2 (Atleti):  Hechicera (Simeone)   — PIEZA REY, ranged, 7 conjuros por partida
// Si el Entrenador es eliminado en arena, el partido termina para ese bando
class Entrenador : public Volador, public Hechicero {
public:
    Entrenador(float x, float y, int equipo)
        : Volador(x, y, equipo, equipo == 1
            ? "../bin/imagenes/fotosjugadores/madridentrenador.png"
            : "../bin/imagenes/fotosjugadores/atletientrenador.png"),
          Hechicero()
    {
        hpMax             = 40;
        velArena          = 130.0;
        danio             = 12;
        cooldownMax       = 1.00;
        disparaRayoArcano = true;
        hp = hpMax;
    }

    // En el tablero se mueve como una reina hasta 3 casillas, volando sobre piezas
    int getRango() const override { return 3; }

    bool   esRanged()      const override { return true; }
    double alcanceAtaque() const override { return 0.0; }

    bool lanzarConjuro(int i, Mundo& m) { return Hechicero::lanzarConjuro(i, m, getEquipo()); }

    bool esEntrenador() const override { return true; }
    void habilidadEspecial() override {}

    bool esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const override;
};
