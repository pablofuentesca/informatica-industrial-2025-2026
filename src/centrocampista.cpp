#include "centrocampista.h"
#include <algorithm>
#include <cmath>

// Valkiria: movimiento en L (como caballo de ajedrez)
bool Valkiria::esMovimientoValido(int ox, int oy, int dx, int dy) const {
    if (ox == dx && oy == dy) return false;
    int difX = std::abs(dx - ox), difY = std::abs(dy - oy);
    return (difX == 2 && difY == 1) || (difX == 1 && difY == 2);
}

// Banshee: cualquier direccion hasta 3 casillas, volando sobre piezas
bool Banshee::esMovimientoValido(int ox, int oy, int dx, int dy) const {
    if (ox == dx && oy == dy) return false;
    int difX = std::abs(dx - ox), difY = std::abs(dy - oy);
    int dist = (std::max)(difX, difY);
    return (difX == 0 || difY == 0 || difX == difY) && dist <= getRango();
}
