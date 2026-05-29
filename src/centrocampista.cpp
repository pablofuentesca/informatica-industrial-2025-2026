#include"centrocampista.h"
#include<algorithm>
#include<cmath>

bool Centrocampista::esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const {
    if (origenX == destinoX && origenY == destinoY) return false;
    int difX = std::abs(destinoX - origenX);
    int difY = std::abs(destinoY - origenY);
    int distancia = (std::max)(difX, difY);

    if (getEquipo() == 1) {
        // MADRID (Valkiria): movimiento en L (caballo de ajedrez), vuela sobre piezas
        return (difX == 2 && difY == 1) || (difX == 1 && difY == 2);
    } else {
        // ATLETI (Banshee): cualquier direccion hasta 3 casillas, vuela sobre piezas
        return (difX == 0 || difY == 0 || difX == difY) && distancia <= getRango();
    }
}