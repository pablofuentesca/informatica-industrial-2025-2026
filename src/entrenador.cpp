#include "entrenador.h"
#include "mundo.h"
#include <algorithm>
#include <cmath>

bool Entrenador::lanzarConjuro(int i, Mundo& m)
{
    if (!tieneConjuros()) return false;
    if (i < 0 || i >= (int)conjuros.size()) return false;
    if (conjuros[i]->estaUsado()) return false;

    if (conjuros[i]->aplicar(m, equipo)) {
        gastaConjuro();
        return true;
    }
    return false;
}

bool Entrenador::esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const {
    if (origenX == destinoX && origenY == destinoY) return false;
    int difX = std::abs(destinoX - origenX);
    int difY = std::abs(destinoY - origenY);
    int distancia = (std::max)(difX, difY);

    if (distancia > getRango()) return false;

    // Mago / Hechicera: Cualquier dirección (Reina)
    return (difX == 0 || difY == 0 || difX == difY);
}