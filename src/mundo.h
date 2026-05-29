#pragma once
#include "tablero.h"
#include "pelota.h"
#include "jugador.h"
#include "grid.h"
#include "equipo.h"
#include "turno.h"

class Mundo {

    Tablero miTablero;
    Pelota balones[5];
    Equipo madrid;
    Equipo atleti;
    Turno  turno;
    Jugador* jugadorSeleccionado;
    Grid<bool, 9> casillasValidas;

    // pieza que ataca y pieza que defiende cuando se produce un enfrentamiento
    Jugador* pendientePj1{ nullptr };
    Jugador* pendientePj2{ nullptr };
    int      destCombateX{ -1 };
    int      destCombateY{ -1 };

    Equipo&       equipoPorId(int id)       { return (id == 1) ? madrid : atleti; }
    const Equipo& equipoPorId(int id) const { return (id == 1) ? madrid : atleti; }

    int  equipoEn(int x, int y) const;
    void calcularCasillasValidas();
    void eliminarPieza(Jugador* pj);
    void cierraTurno();   // cambia turno + avanza ciclo + descuenta encarcelados

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
    void invertirCiclo();
    void curarEquipoCompleto(int equipo);
    bool teleportarAleatoriamente(int equipo);
    bool intercambiarPiezas(int equipo);
    bool invocarElemental(int equipo);
    bool revivirPieza(int equipo);
    bool encarcelarEnemigo(int equipo);
    int  comprobarVictoria() const;
};
