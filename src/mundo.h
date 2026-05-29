#pragma once
#include <vector>
#include "tablero.h"
#include "pelota.h"
#include "jugador.h"
#include "grid.h"

class Mundo {

    Tablero miTablero;
    Pelota balones[5];
    std::vector<Jugador*> equipoMadrid;
    std::vector<Jugador*> equipoAtleti;
    std::vector<Jugador*> cementerioMadrid;
    std::vector<Jugador*> cementerioAtleti;
    Jugador* jugadorSeleccionado;
    Grid<bool, 9> casillasValidas;
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
    void actualizarEncarcelados();

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
