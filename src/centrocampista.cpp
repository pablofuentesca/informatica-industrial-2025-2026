#include"centrocampista.h"
#include<algorithm>

bool Centrocampista::esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const {
    if (origenX == destinoX && origenY == destinoY) return false;
    int difX = std::abs(destinoX - origenX);
    int difY = std::abs(destinoY - origenY);

    // Valkiria / Banshee: 1 paso ortogonal (Cruz)
    return (difX + difY == 1);
}