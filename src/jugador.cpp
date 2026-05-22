#include "jugador.h"
#include "ETSIDI.h"

Jugador::Jugador(float x, float y, int _equipo, RolFutbol _rol) {
    radio = 0.4f;
    pos.x = x + 0.5f;
    pos.y = y + 0.5f;
    equipo = _equipo;
    rol = _rol;

    // Configuramos las stats de Archon según el rol que le pasemos
    switch (rol) {
    case PORTERO:        rangoMovimiento = 3; esVolador = false; break;
    case DELANTERO:      rangoMovimiento = 3; esVolador = false; break;
    case CENTRAL:        rangoMovimiento = 3; esVolador = false; break;
    case LATERAL:        rangoMovimiento = 4; esVolador = false; break;
    case CENTROCAMPISTA: rangoMovimiento = 3; esVolador = true;  break;
    case MEDIAPUNTA:     rangoMovimiento = 4; esVolador = true;  break;
    case EXTREMO:        rangoMovimiento = 5; esVolador = true;  break;
    case ENTRENADOR:     rangoMovimiento = 3; esVolador = false; break; // El mago
    }

    // Cargamos la imagen según el equipo
    if (equipo == 1) {
        sprite = new ETSIDI::Sprite("../bin/imagenes/madridcentro.png", pos.x, pos.y, radio * 2, radio * 2);
    }
    else {
        sprite = new ETSIDI::Sprite("../bin/imagenes/atleticentro.png", pos.x, pos.y, radio * 2, radio * 2);
    }
}

Jugador::~Jugador() { delete sprite; }

void Jugador::dibuja() { sprite->draw(); }

void Jugador::mover(float dirX, float dirY) {
    float nuevaX = pos.x + dirX;
    float nuevaY = pos.y + dirY;

    // Comprobamos que no se salga de los límites del tablero (casillas 0 a 8)
    if (nuevaX > 0.0f && nuevaX < 9.0f && nuevaY > 0.0f && nuevaY < 9.0f) {
        pos.x = nuevaX;
        pos.y = nuevaY;
        sprite->setPos(pos.x, pos.y);
    }
}