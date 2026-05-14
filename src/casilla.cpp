#include "casilla.h"

// Constructor: por defecto la hacemos blanca (1, 1, 1 en OpenGL)
Casilla::Casilla() {
    rojo = 1.0f;
    verde = 1.0f;
    azul = 1.0f;
}

Casilla::~Casilla() {}

void Casilla::setColor(float r, float g, float b) {
    rojo = r;
    verde = g;
    azul = b;
}

// Aquí meteremos el código de OpenGL (GL_QUADS) más adelante
void Casilla::dibuja() {}