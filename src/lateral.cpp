#include "lateral.h"
#include <cmath>

bool Lateral::esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const {
    if (origenX == destinoX && origenY == destinoY) return false;

    int difX = std::abs(destinoX - origenX);
    int difY = std::abs(destinoY - origenY);

    // Matemática pura del salto en "L" del ajedrez
    return (difX == 2 && difY == 1) || (difX == 1 && difY == 2);
}