#include "delantero.h"
#include <cmath>

bool Delantero::esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const {
    if (origenX == destinoX && origenY == destinoY) return false;
    int difX = std::abs(destinoX - origenX);
    int difY = std::abs(destinoY - origenY);

    if (getEquipo() == 1) {
        // MADRID (Caballeros): Movimiento en "L"
        return (difX == 2 && difY == 1) || (difX == 1 && difY == 2);
    }
    else {
        // ATLETI (Goblins): 1 paso en diagonal
        return (difX == 1 && difY == 1);
    }
}
