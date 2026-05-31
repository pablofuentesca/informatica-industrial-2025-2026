#include "mediapunta.h"
#include <algorithm>
#include <cmath>

// Genio y Cambiaformas: movimiento diagonal hasta 4 casillas (Alfil)
bool Mediapunta::esMovimientoValido(int ox, int oy, int dx, int dy) const {
    if (ox == dx && oy == dy) return false;
    int difX = std::abs(dx - ox), difY = std::abs(dy - oy);
    if ((std::max)(difX, difY) > getRango()) return false;
    return (difX == difY);
}
