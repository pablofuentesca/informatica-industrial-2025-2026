#pragma once
#include "teletransportador.h"

// Equipo 1 (Madrid):  Mago     (Ancelotti) — PIEZA REY, ranged, 6 conjuros por partida
// Equipo 2 (Atleti):  Hechicera (Simeone)  — PIEZA REY, ranged, 6 conjuros por partida
// Si el Entrenador es eliminado en arena, el partido termina para ese bando
class Entrenador : public Teletransportador {
    int conjurosRestantes{ 6 };

public:
    Entrenador(float x, float y, int equipo)
        : Teletransportador(x, y, equipo, equipo == 1
            ? "../bin/imagenes/fotosjugadores/madridentrenador.png"
            : "../bin/imagenes/fotosjugadores/atletientrenador.png")
    {
        // Mago y Hechicera comparten estadisticas: frágiles pero con magia poderosa
        hpMax       = 40;
        velArena    = 130.0;
        danio       = 12;
        cooldownMax = 1.00;
        hp = hpMax;
    }

    // En el tablero puede teletransportarse a cualquier casilla libre
    int getRango() const override { return 3; }

    bool   esRanged()      const override { return true; }
    double alcanceAtaque() const override { return 0.0; }

    // Movimiento: en el tablero se teletransporta en lugar de desplazarse
    void mover(float dirX, float dirY) override {
        Jugador::mover(dirX, dirY);
    }

    // Conjuros: el Mundo/Coordinador pregunta cuantos quedan y llama a gastaConjuro()
    int  getConjurosRestantes() const { return conjurosRestantes; }
    bool tieneConjuros()        const { return conjurosRestantes > 0; }

    bool gastaConjuro() {
        if (conjurosRestantes <= 0) return false;
        conjurosRestantes--;
        return true;
    }

    // habilidadEspecial: lanza el conjuro seleccionado actualmente
    // La logica concreta de cada conjuro (Heal, ShiftTime, etc.) la gestiona el Coordinador
    void habilidadEspecial() override {}
};
