#pragma once
#include "volador.h"

// Equipo 1 (Madrid):  Valkiria (Vinicius) — mele aereo rapido, supera obstaculos volando
// Equipo 2 (Atleti):  Banshee  (De Paul)  — ranged aereo, su proyectil atraviesa obstaculos
class Centrocampista : public Volador {
public:
    Centrocampista(float x, float y, int equipo)
        : Volador(x, y, equipo, equipo == 1
            ? "../bin/imagenes/fotosjugadores/madridcentro.png"
            : "../bin/imagenes/fotosjugadores/atleticentro.png")
    {
        if (equipo == 1) {
            // Valkiria: la mas rapida del bando luz, ataca en diagonal mele
            hpMax       = 70;
            velArena    = 240.0;
            danio       = 18;
            cooldownMax = 0.60;
        } else {
            // Banshee: rapida, su alarido (proyectil) atraviesa barreras y charcos
            hpMax       = 65;
            velArena    = 240.0;
            danio       = 16;
            cooldownMax = 0.70;
        }
        hp = hpMax;
    }

    int  getRango() const override { return 3; }

    // Valkiria (eq1): mele — espada que alcanza un poco mas alla del cuerpo
    // Banshee  (eq2): ranged — alarido que atraviesa obstaculos del campo
    bool   esRanged()      const override { return equipo == 2; }
    double alcanceAtaque() const override { return (equipo == 1) ? 30.0 : 0.0; }

    // Ambas son voladoras: en el tablero vuelan sobre otras piezas
    // En la arena ignoran la colision con barreras y charcos al moverse
    void mover(float dirX, float dirY) override {
        Jugador::mover(dirX, dirY);
    }

    // Banshee (equipo 2): su proyectil tiene la marca "atraviesaObstaculos"
    // que la Arena comprueba al actualizar los proyectiles en vuelo
    void habilidadEspecial() override {}
};
