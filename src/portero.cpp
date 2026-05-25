#include"portero.h"
#include<algorithm>
#include<cmath>

bool Portero::esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const {
    if (origenX == destinoX && origenY == destinoY) return false;
    int difX = std::abs(destinoX - origenX);
    int difY = std::abs(destinoY - origenY);
    int distancia = (std::max)(difX, difY);

    if (getEquipo() == 1) {
        // MADRID (Arquera): Diagonal (Alfil)
        if (distancia > getRango()) return false;
        return (difX == difY);
    }
    else {
        // ATLETI (Mantícora/Grifo): 2 o 3 pasos en cualquier dirección
        return (difX == 0 || difY == 0 || difX == difY) && (distancia >= 2 && distancia <= 3);
    }
}