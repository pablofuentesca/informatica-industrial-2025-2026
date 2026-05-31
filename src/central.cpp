#include "central.h"
#include <cmath>
#include <algorithm>

// Golem y Trol: movimiento ortogonal (Torre)
bool Central::esMovimientoValido(int ox, int oy, int dx, int dy) const {
    if (ox == dx && oy == dy) return false;
    int difX = std::abs(dx - ox), difY = std::abs(dy - oy);
    if ((std::max)(difX, difY) > getRango()) return false;
    return (difX == 0 || difY == 0);
}
