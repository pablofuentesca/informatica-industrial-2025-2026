#pragma once
#include "posicion.h"

namespace ETSIDI { class Sprite; }

// roles
enum RolFutbol { PORTERO, DELANTERO, CENTRAL, LATERAL, CENTROCAMPISTA, MEDIAPUNTA, EXTREMO, ENTRENADOR };

class Jugador {

    float radio;
    ETSIDI::Sprite* sprite;
    int equipo;
    RolFutbol rol;

    // Estadísticas de Archon
    int rangoMovimiento;
    bool esVolador;

public:
    Jugador(float x, float y, int _equipo, RolFutbol _rol);
    ~Jugador();

    Posicion pos; 

    void dibuja() const;
    void mover(float dirX, float dirY);
};