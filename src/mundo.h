#pragma once
#include "tablero.h"
#include "pelota.h"
#include "jugador.h"

class Mundo {
private:
    Tablero miTablero;
    Pelota balones[5];
    Jugador* equipoMadrid[18];
    Jugador* equipoAtleti[18];

public:
    Mundo();
    ~Mundo();

    void inicializa();
    void dibuja() const;
    void mueve();
    void tecla(unsigned char key);
    void teclaEspecial(int key);
};
