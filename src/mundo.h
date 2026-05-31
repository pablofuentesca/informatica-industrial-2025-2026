#pragma once
#include "tablero.h"
#include "pelota.h"
#include "jugador.h"
#include "grid.h"
#include "equipo.h"
#include "turno.h"
#include <map>

class VistaJugador;   // declaracion anticipada: solo usamos VistaJugador* en el mapa
class IA;
class Mundo {
    friend class IA;

    Tablero miTablero;
    Pelota balones[5];
    Equipo madrid;
    Equipo atleti;
    Turno  turno;
    Jugador* jugadorSeleccionado;
    Grid<bool,9> casillasValidas;

    enum class FaseConjuro { NINGUNO, TELEPORT_PIEZA, TELEPORT_DESTINO, EXCHANGE_PRIMERA, EXCHANGE_SEGUNDA };
    FaseConjuro faseConjuro{ FaseConjuro::NINGUNO };
    Jugador* piezaConjuro{ nullptr };
    int  equipoConjuro{ 0 };
    bool mostrarHechizos{ false };

    // pieza que ataca y pieza que defiende cuando se produce un enfrentamiento
    Jugador* pendientePj1{ nullptr };
    Jugador* pendientePj2{ nullptr };
    int destCombateX{ -1 };
    int destCombateY{ -1 };

    // mapa vista grafica de cada pieza: el sprite vive aqui, no en Jugador
    std::map<const Jugador*, VistaJugador*> vistas;

    Equipo& equipoPorId(int id){ return (id == 1) ? madrid : atleti; }
    const Equipo& equipoPorId(int id) const { return (id == 1) ? madrid : atleti; }

    int equipoEn(int x, int y) const;
    bool esPuntoDePoder(int x, int y) const; // las 5 casillas de poder del tablero
    void curarEnPuntosDePoder(); // regenera vida a las piezas situadas en ellas
    void calcularCasillasValidas();
    void eliminarPieza(Jugador* pj);
    void cierraTurno(); // cambia turno + avanza ciclo + descuenta encarcelados

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
    bool hayCombatePendiente() const { return pendientePj1 != nullptr; }
    Jugador* getCombatiente1() const { return pendientePj1; }
    Jugador* getCombatiente2() const { return pendientePj2; }
    void resolverCombate(int equipoGanador);
    void limpiarCombatePendiente();
    int ventajaCombate() const;
    void invertirCiclo();
    bool curarUnaPieza(int equipo);
    bool teleportarAleatoriamente(int equipo);
    bool intercambiarPiezas(int equipo);
    bool invocarElemental(int equipo);
    bool revivirPieza(int equipo);
    bool encarcelarEnemigo(int equipo);
    int comprobarVictoria() const;
};
