#include "Casilla.h"
#include "freeglut.h" // Librería necesaria para dibujar

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

void Casilla::dibuja() const {
    // Definimos el tamaño de la casilla (un cuadrado de lado 1)
    float lado = 1.0f;

    // Le decimos a OpenGL el color de esta casilla específica
    glColor3f(rojo, verde, azul);

    // Dibujamos el cuadrado usando las coordenadas de su posición
    glBegin(GL_QUADS);
    glVertex2f(pos.x, pos.y);              // Esquina inferior izquierda
    glVertex2f(pos.x + lado, pos.y);       // Esquina inferior derecha
    glVertex2f(pos.x + lado, pos.y + lado);// Esquina superior derecha
    glVertex2f(pos.x, pos.y + lado);       // Esquina superior izquierda
    glEnd();
}