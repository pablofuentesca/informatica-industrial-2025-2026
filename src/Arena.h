#pragma once
#include "jugador.h"

// Obstaculo del campo de combate
struct Obstaculo {
    double x, y;
    double ancho, alto;
    bool esBanderin;    // false=barrera, true=charco de barro
    bool activo;
    double anchoVisual, altoVisual;
    double tAparecer;
    double tVida;

    Obstaculo() : x(0), y(0), ancho(40), alto(30), esBanderin(false),
                  activo(false), anchoVisual(40), altoVisual(30),
                  tAparecer(0), tVida(0) {}
};

class Arena {
    double xMin, xMax, yMin, yMax;
    double tam;

    double j1x, j1y;   // posicion en arena del combatiente equipo 1 (WASD)
    double j2x, j2y;   // posicion en arena del combatiente equipo 2 (flechas)
    double arbY;

    double t;
    bool silbatoSonado;
    bool usarDiagonal;

    Obstaculo obstaculos[8];
    double tBatalla;

    bool j1Arr, j1Aba, j1Izq, j1Der;
    bool j2Arr, j2Aba, j2Izq, j2Der;

    Jugador* j1ptr;   // pieza real equipo 1 (Madrid, WASD)
    Jugador* j2ptr;   // pieza real equipo 2 (Atleti, flechas)

    enum EstadoArena {
        TRANSICION,
        ENTRADA,
        ARBITRO_ENTRANDO,
        ARBITRO_ESPERA,
        ARBITRO_SALIENDO,
        BATALLA
    } estado;

    enum Resultado { EN_CURSO, GANA_J1, GANA_J2 } resultado;

    friend class interaccionArena;

public:
    Arena() : xMin(50), xMax(750), yMin(50), yMax(550),
              tam(25),
              j1x(-50), j1y(300), j2x(850), j2y(300),
              arbY(650), t(0.0), silbatoSonado(false), usarDiagonal(true),
              tBatalla(0.0),
              j1Arr(false), j1Aba(false), j1Izq(false), j1Der(false),
              j2Arr(false), j2Aba(false), j2Izq(false), j2Der(false),
              j1ptr(nullptr), j2ptr(nullptr),
              estado(TRANSICION), resultado(EN_CURSO) {}

    // atacante: pieza que se movio; defensor: pieza que ocupaba la casilla
    void inicializa(Jugador* atacante, Jugador* defensor);
    void dibuja() const;
    void mueve(double dt);
    void tecla(unsigned char key);
    void teclaJ1(unsigned char key);
    void teclaEspecial(int key);
    void teclaEspecialArriba(int key);

    bool     termino()     const { return resultado != EN_CURSO; }
    Jugador* getPerdedor() const;
    Jugador* getGanador()  const;
};
