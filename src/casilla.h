#pragma once
#include "Posicion.h"

class Casilla {
public:
    // La coordenada de la esquina o centro de la baldosa
    Posicion pos;

    // Podemos guardar el color (por si quieres hacer un tablero estilo ajedrez)
    float rojo, verde, azul;

    Casilla();
    ~Casilla();

    // Métodos para configurar y dibujar la casilla
    void setColor(float r, float g, float b);
    void dibuja();
}; 
