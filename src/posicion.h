#pragma once

class Posicion {
public:
    // Coordenadas
    float x;
    float y;

    // Constructores
    Posicion();
    Posicion(float _x, float _y);

    // Método para cambiar la posición rápidamente
    void set(float _x, float _y);
};