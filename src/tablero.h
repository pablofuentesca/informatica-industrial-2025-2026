#pragma once
#include "casilla.h"

class Tablero {
    // Matriz bidimensional de 9 columnas x 9 filas (81 casillas en total)
    Casilla casillas[9][9];

    // Estado del ciclo de luminiscencia (Archon "shifting board"):
    //   cicloLuz: 0 = extremo oscuro ... 6 = extremo claro, 3 = neutro
    //   direccion: +1 si el ciclo se mueve hacia mas claro, -1 hacia mas oscuro.
    //              Rebota al llegar a los extremos 0 y 6.
    int cicloLuz;
    int direccion;

    void propagaCicloACasillas();

public:
    Tablero();

    void inicializa();
    void dibuja() const;

    // Llamar al cerrar cada turno (movimiento, combate resuelto, conjuro shift time).
    void avanzarCiclo();

    // Permite que un conjuro tipo "Shift Time" invierta el ciclo (normativa, l. 299).
    void invierteDireccionCiclo() { direccion = -direccion; }

    int  getCicloLuz()      const { return cicloLuz; }
    int  getDireccion()     const { return direccion; }

    // Consulta de ventaja para combate / curacion segun la casilla (gx, gy).
    bool casillaFavoreceLuz   (int gx, int gy) const;
    bool casillaFavoreceOscuro(int gx, int gy) const;
};
