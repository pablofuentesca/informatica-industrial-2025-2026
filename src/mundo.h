#pragma once
#include "tablero.h"
#include "pelota.h"
#include "jugador.h"

class Mundo {

    Tablero miTablero;
    Pelota balones[5];
    Jugador* equipoMadrid[18];
    Jugador* equipoAtleti[18];
    Jugador* jugadorSeleccionado;
    bool casillasValidas[9][9];
    int  turnoEquipo{ 1 };   // 1 = Madrid/Blanco, 2 = Atleti/Rojo
    bool primerTurno{ true };   

    int  equipoEn(int x, int y) const;
    void calcularCasillasValidas();

public:
    Mundo();
    ~Mundo();

    void inicializa();
    void dibuja() const;
    void mueve();
    void tecla(unsigned char key);
    void teclaEspecial(int key);
    void raton(int boton, int estado, float x, float y);
};
