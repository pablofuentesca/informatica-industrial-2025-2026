#include "lateral.h"
#include <cmath>
#include <algorithm>

// Unicornio: cualquier direccion hasta 2 casillas (ortogonal o diagonal)
bool Unicornio::esMovimientoValido(int ox, int oy, int dx, int dy) const {
    if (ox == dx && oy == dy) return false;
    int difX = std::abs(dx - ox), difY = std::abs(dy - oy);
    if ((std::max)(difX, difY) > getRango()) return false;
    return (difX == 0 || difY == 0 || difX == difY);
}

// Basilisco: solo diagonal hasta 4 casillas
bool Basilisco::esMovimientoValido(int ox, int oy, int dx, int dy) const {
    if (ox == dx && oy == dy) return false;
    int difX = std::abs(dx - ox), difY = std::abs(dy - oy);
    if ((std::max)(difX, difY) > getRango()) return false;
    return (difX == difY);
}
