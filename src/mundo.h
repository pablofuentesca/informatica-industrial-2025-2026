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

    // pieza que ataca y pieza que defiende cuando se produce un enfrentamiento
    Jugador* pendientePj1{ nullptr };
    Jugador* pendientePj2{ nullptr };
    int      destCombateX{ -1 };
    int      destCombateY{ -1 };

    int  equipoEn(int x, int y) const;
    void calcularCasillasValidas();
    void eliminarPieza(Jugador* pj);

public:
    Mundo();
    ~Mundo();

    void inicializa();
    void dibuja() const;
    void mueve();
    void tecla(unsigned char key);
    void teclaEspecial(int key);
    void raton(int boton, int estado, float x, float y);

    // combate iniciado desde el tablero
    bool     hayCombatePendiente() const { return pendientePj1 != nullptr; }
    Jugador* getCombatiente1()     const { return pendientePj1; }
    Jugador* getCombatiente2()     const { return pendientePj2; }
    void resolverCombate(int equipoGanador);
    void limpiarCombatePendiente();
};
