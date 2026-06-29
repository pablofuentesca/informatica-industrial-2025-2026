#pragma once
#include "mundo.h"
#include "Arena.h"
#include "ia.h"
#include "ranking.h"
#include <string>

// Solo datos compartidos entre pantallas. Nada de logica de juego aqui.
struct Contexto {
    enum Id { NINGUNA, INICIO, REGLAS, NOMBRES, JUEGO, COMBATE, FIN, RANKING };

    Mundo mundo;
    Arena arena;
    IA ia;
    Ranking ranking{ "../bin/ranking.txt" };
    bool  modoIA{ false };
    int  equipoVencedor{ 0 };
    std::string nombreMadrid;
    std::string nombreAtleti;

    Id   siguiente{ NINGUNA }; 
    void pide(Id id) { siguiente = id; }
};
