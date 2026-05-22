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
    //Calculamos dónde caería si da el paso
    float nuevaX = pos.x + dirX;
    float nuevaY = pos.y + dirY;

    //Comprobamos si esa nueva posición está dentro del césped
    // Como los centros de las casillas van de 0.5 a 8.5, comprobamos que no baje de 0 ni pase de 9
    if (nuevaX > 0.0f && nuevaX < 9.0f && nuevaY > 0.0f && nuevaY < 9.0f) {

        // Si es legal, actualizamos su posición de verdad
        pos.x = nuevaX;
        pos.y = nuevaY;

        // Y actualizamos el dibujo
        sprite->setPos(pos.x, pos.y);
    }
}