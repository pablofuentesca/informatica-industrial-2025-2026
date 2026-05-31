#include "entrenador.h"
#include <algorithm>
#include <cmath>

// Mago y Hechicera: movimiento de reina hasta 3 casillas (cualquier direccion)
bool Entrenador::esMovimientoValido(int ox, int oy, int dx, int dy) const {
    if (ox == dx && oy == dy) return false;
    int difX = std::abs(dx - ox), difY = std::abs(dy - oy);
    if ((std::max)(difX, difY) > getRango()) return false;
    return (difX == 0 || difY == 0 || difX == difY);
}
