#include "delantero.h"
#include <cmath>

// Caballero: movimiento en L (como caballo de ajedrez)
bool Caballero::esMovimientoValido(int ox, int oy, int dx, int dy) const
{
    if (ox == dx && oy == dy) return false;
    int difX = std::abs(dx - ox), difY = std::abs(dy - oy);
    return (difX == 2 && difY == 1) || (difX == 1 && difY == 2);
}

// Goblin: 1 paso en diagonal
bool Goblin::esMovimientoValido(int ox, int oy, int dx, int dy) const {
    if (ox == dx && oy == dy) return false;
    int difX = std::abs(dx - ox), difY = std::abs(dy - oy);
    return (difX == 1 && difY == 1);
}
