#pragma once
#include "casilla.h"

class Tablero {
private:
    //Matriz bidimensional de 9 columnas x 9 filas (81 casillas en total)
	Casilla casillas[9][9];//Creamos un objeto de tipo Casilla para cada una de las 81 casillas del tablero

public:
    Tablero();
    ~Tablero();

    void inicializa();
    void dibuja();
};
