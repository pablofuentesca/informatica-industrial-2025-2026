#include "Tablero.h"

Tablero::Tablero() {}

Tablero::~Tablero() {}

void Tablero::inicializa() {
    // Mapa exacto del tablero de la imagen de Archon
    // 'W' = Blanco (Líneas)
    // 'B' = Verde Oscuro (Sustituye al Negro)
    // 'G' = Verde Claro (Sustituye al Gris)
    char mapa[9][9] = {
        {'B', 'G', 'B', 'W', 'W', 'W', 'G', 'B', 'G'}, // Fila 0 (Abajo)
        {'G', 'B', 'W', 'G', 'W', 'B', 'W', 'G', 'B'}, // Fila 1
        {'B', 'W', 'G', 'B', 'W', 'G', 'B', 'W', 'G'}, // Fila 2
        {'W', 'G', 'B', 'G', 'W', 'B', 'G', 'B', 'W'}, // Fila 3
        {'G', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'B'}, // Fila 4 (Centro)
        {'W', 'G', 'B', 'G', 'W', 'B', 'G', 'B', 'W'}, // Fila 5
        {'B', 'W', 'G', 'B', 'W', 'G', 'B', 'W', 'G'}, // Fila 6
        {'G', 'B', 'W', 'G', 'W', 'B', 'W', 'G', 'B'}, // Fila 7
        {'B', 'G', 'B', 'W', 'W', 'W', 'G', 'B', 'G'}  // Fila 8 (Arriba)
    };

    // Recorremos la matriz para crear las casillas
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            // Posición matemática: i (columnas/X), j (filas/Y)
            casillas[i][j].pos.x = (float)i;
            casillas[i][j].pos.y = (float)j;

            // Leemos el color de nuestro mapa (j es la fila, i es la columna)
            char color = mapa[j][i];

            if (color == 'W') {
                // BLANCO
                casillas[i][j].setColor(1.0f, 1.0f, 1.0f);
            }
            else if (color == 'B') {
                // VERDE OSCURO (Sustituto del Negro)
                casillas[i][j].setColor(0.0f, 0.2f, 0.0f);
            }
            else if (color == 'G') {
                // VERDE CLARO (Sustituto del Gris)
                casillas[i][j].setColor(0.3f, 0.8f, 0.3f);
            }
        }
    }
}

void Tablero::dibuja() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            casillas[i][j].dibuja();
        }
    }
}