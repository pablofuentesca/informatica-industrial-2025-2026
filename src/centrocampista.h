#pragma once
#include "jugadores.h"

// Preparamos el terreno para ETSIDI
namespace ETSIDI { class Sprite; }

class Centrocampista : public Jugadores {

    float radio;
    ETSIDI::Sprite* sprite;
    int equipo; // 1 para el Real Madrid (Luz), 2 para el Atleti (Oscuridad)

public:
    Centrocampista(float x, float y, int _equipo);
    ~Centrocampista();

    void dibuja();
    //función para saltar de casilla en casilla
    void mover(float dirX, float dirY);
};