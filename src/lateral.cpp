#include "lateral.h"
#include <cmath>
#include<algorithm>

bool Lateral::esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const {
    if (origenX == destinoX && origenY == destinoY) return false;
    int difX = std::abs(destinoX - origenX);
    int difY = std::abs(destinoY - origenY);
    int distancia = (std::max)(difX, difY);

    if (distancia > getRango()) return false;

    if (getEquipo() == 1) {
        // MADRID (Unicornio): Movimiento en "L"
        return (difX == 2 && difY == 1) || (difX == 1 && difY == 2);
    }
    else {
        // ATLETI (Basilisco): Diagonal
        return (difX == difY);
    }
}