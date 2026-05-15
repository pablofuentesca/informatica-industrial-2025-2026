#include "tablero.h"
//sergio mamahuevo
Tablero::Tablero() {}

Tablero::~Tablero() {}

void Tablero::inicializa() {
    // Recorremos las 9 columnas (i) y las 9 filas (j)
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            //Le damos a cada casilla su posición en la cuadrícula
            casillas[i][j].pos.x = (float)i;
            casillas[i][j].pos.y = (float)j;

			//Alternamos el color de las casillas para crear un tablero estilo ajedrez, pero usando tonalidades de verde que simulen un campo de fútbol
            if ((i + j) % 2 == 0) {
                casillas[i][j].setColor(0.3f, 0.8f, 0.3f);
            }
            else {
                casillas[i][j].setColor(0.0f, 0.2f, 0.0f);
            }
        }
    }
}

void Tablero::dibuja() {
    // Le decimos a cada una de las 81 casillas que se dibuje
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            casillas[i][j].dibuja();
        }
    }
}