#include "portero.h"
#include <cmath>
#include <algorithm>

// Arquero: movimiento diagonal hasta 3 casillas (Alfil)
bool Arquero::esMovimientoValido(int ox, int oy, int dx, int dy) const {
    if (ox == dx && oy == dy) return false;
    int difX = std::abs(dx - ox), difY = std::abs(dy - oy);
    if ((std::max)(difX, difY) > getRango()) return false;
    return (difX == difY);
}

// Manticora: 2 o 3 pasos en cualquier direccion
bool Manticora::esMovimientoValido(int ox, int oy, int dx, int dy) const {
    if (ox == dx && oy == dy) return false;
    int difX = std::abs(dx - ox), difY = std::abs(dy - oy);
    int dist = (std::max)(difX, difY);
    return (difX == 0 || difY == 0 || difX == difY) && (dist >= 2 && dist <= 3);
}
