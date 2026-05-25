#include"extremo.h"
#include<algorithm>
#include<cmath>

bool Extremo::esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const {
    if (origenX == destinoX && origenY == destinoY) return false;
    int difX = std::abs(destinoX - origenX);
    int difY = std::abs(destinoY - origenY);
    int distancia = (std::max)(difX, difY);

    if (distancia > getRango()) return false;

    if (getEquipo() == 1) {
        // MADRID (Fénix): Cualquier dirección (Reina)
        return (difX == 0 || difY == 0 || difX == difY);
    }
    else {
        // ATLETI (Dragón): Ortogonal (Torre)
        return (difX == 0 || difY == 0);
    }
}