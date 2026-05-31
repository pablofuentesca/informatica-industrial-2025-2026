#include "extremo.h"
#include <algorithm>
#include <cmath>

// Fenix: se mueve como reina de ajedrez (cualquier direccion)
bool Fenix::esMovimientoValido(int ox, int oy, int dx, int dy) const {
    if (ox == dx && oy == dy) return false;
    int difX = std::abs(dx - ox), difY = std::abs(dy - oy);
    if ((std::max)(difX, difY) > getRango()) return false;
    return (difX == 0 || difY == 0 || difX == difY);
}

// Dragon: se mueve como torre de ajedrez (solo ortogonal)
bool Dragon::esMovimientoValido(int ox, int oy, int dx, int dy) const {
    if (ox == dx && oy == dy) return false;
    int difX = std::abs(dx - ox), difY = std::abs(dy - oy);
    if ((std::max)(difX, difY) > getRango()) return false;
    return (difX == 0 || difY == 0);
}
