#include "posicion.h"

// Constructor por defecto (si no le decimos nada, va al centro 0,0)
Posicion::Posicion() {
    x = 0.0f;
    y = 0.0f;
}

// Constructor pasándole las coordenadas que queramos
Posicion::Posicion(float _x, float _y) {
    x = _x;
    y = _y;
}

// Método para actualizar las coordenadas
void Posicion::set(float _x, float _y) {
    x = _x;
    y = _y;
}