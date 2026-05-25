#include"central.h"
#include<cmath>
#include<algorithm>

bool Central::esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const {
    if (origenX == destinoX && origenY == destinoY) return false;
    int difX = std::abs(destinoX - origenX);
    int difY = std::abs(destinoY - origenY);
    int distancia = (std::max)(difX, difY);

    if (distancia > getRango()) return false;

    // Golem / Troll: Ortogonal (Torre)
    return (difX == 0 || difY == 0);
}