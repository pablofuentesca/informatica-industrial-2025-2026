#include "delantero.h"
#include <cmath>

bool Delantero::esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const {
    if (origenX == destinoX && origenY == destinoY) return false;

    int difX = std::abs(destinoX - origenX);
    int difY = std::abs(destinoY - origenY);

    // Regla del Goblin/Enano:
    return (difX == 1 && difY == 1);
}
