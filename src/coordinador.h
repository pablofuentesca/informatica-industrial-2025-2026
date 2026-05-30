#pragma once
#include "mundo.h"
#include "Arena.h"
#include "ia.h"
#include "ranking.h"
#include <string>

namespace ETSIDI { class Sprite; }

class Coordinador {
    enum Estado { INICIO, REGLAS, NOMBRES, JUEGO, COMBATE, PAUSA, FIN, RANKING } estado{};
    Mundo mundo;
    Arena arena;
    IA ia;
    Ranking ranking{ "../bin/ranking.txt" };
    bool modoIA{ false };
    ETSIDI::Sprite* portada{ nullptr };
    int equipoVencedor{ 0 };

    // nombres que los jugadores introducen al empezar (para el ranking)
    std::string nombreMadrid;
    std::string nombreAtleti;
    std::string entradaActual;    // texto que se esta tecleando ahora
    int         campoNombre{ 0 }; // 0 = nombre de Madrid, 1 = nombre de Atleti

    void iniciaEntradaNombres(bool conIA);   // prepara la pantalla de nombres

public:
    Coordinador() = default;
    ~Coordinador();
    Coordinador(const Coordinador&) = delete;
    Coordinador& operator=(const Coordinador&) = delete;
    void inicializa();
    void dibuja() const;
    void mueve(double dt);
    void tecla(unsigned char key);
    void teclaJ1(unsigned char key);
    void teclaEspecial(int key);
    void teclaEspecialArriba(int key);
    void raton(int boton, int estadoRat, int x, int y);
};
