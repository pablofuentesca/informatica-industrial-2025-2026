#include "centrocampista.h"
#include "ETSIDI.h"

Centrocampista::Centrocampista(float x, float y, int _equipo) {
    radio = 0.4f; // Un pelín más grande que la pelota para que destaque
    pos.x = x + 0.5f;
    pos.y = y + 0.5f;
    equipo = _equipo;

    // Dependiendo del equipo, le cargamos una equipación u otra
    if (equipo == 1) {
        
        sprite = new ETSIDI::Sprite("../bin/imagenes/madridcentro.png", pos.x, pos.y, radio * 2, radio * 2);
    }
    else {
        
        sprite = new ETSIDI::Sprite("../bin/imagenes/atleticentro.png", pos.x, pos.y, radio * 2, radio * 2);
    }
}

Centrocampista::~Centrocampista() {
    delete sprite;
}

void Centrocampista::dibuja() {
    sprite->draw();
}

void Centrocampista::mover(float dirX, float dirY) {
    pos.x += dirX;
    pos.y += dirY;

    // Le decimos a ETSIDI que actualice la foto a la nueva coordenada
    sprite->setPos(pos.x, pos.y);
}