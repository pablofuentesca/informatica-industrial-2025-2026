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
        // MADRID (Unicornio): cualquier direccion hasta 2 casillas (ortogonal o diagonal)
        return (difX == 0 || difY == 0 || difX == difY);
    }
    else {
        // ATLETI (Basilisco): solo diagonal, hasta 4 casillas
        return (difX == difY);
    }
}