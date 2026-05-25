#include"mediapunta.h"
#include<algorithm>
#include<cmath>

bool Mediapunta::esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const {
    if (origenX == destinoX && origenY == destinoY) return false;
    int difX = std::abs(destinoX - origenX);
    int difY = std::abs(destinoY - origenY);
    int distancia = (std::max)(difX, difY);

    if (distancia > getRango()) return false;

    // Genio / Cambiaformas: Diagonal (Alfil)
    return (difX == difY);
}
