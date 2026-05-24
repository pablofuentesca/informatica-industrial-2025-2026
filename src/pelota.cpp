#include "pelota.h"
#include "ETSIDI.h" // Incluimos la librería de la asignatura

Pelota::Pelota(float x, float y) {
    radio = 0.35f;
    pos.x = x + 0.5f;
    pos.y = y + 0.5f;

    // Inicializamos el sprite pasándole:
    // 1. La ruta de la imagen
    // 2. Posición X e Y
    // 3. Ancho y Alto (el doble del radio)
    sprite = new ETSIDI::Sprite("../bin/imagenes/pelota.png", pos.x, pos.y, radio * 2, radio * 2);
}

Pelota::~Pelota() {
    // Como hemos usado "new", hay que destruirlo con "delete"
    delete sprite;
}

void Pelota::dibuja() const {
    // ETSIDI dibuja la imagen con una sola línea de código
    sprite->draw();
}

void Pelota::mueve(float t) {
    // Actualizamos la posición del sprite a la de nuestra clase
    sprite->setPos(pos.x, pos.y);
}