#pragma once
#include "mundo.h"
#include "Arena.h"
#include "ia.h"

namespace ETSIDI { class Sprite; }

class Coordinador {
    enum Estado { INICIO, REGLAS, JUEGO, COMBATE, PAUSA, FIN } estado{};
    Mundo mundo;
    Arena arena;
    IA ia;
    bool modoIA{ false };
    ETSIDI::Sprite* portada{ nullptr };
    int equipoVencedor{ 0 };

public:
    ~Coordinador();
    void inicializa();
    void dibuja() const;
    void mueve(double dt);
    void tecla(unsigned char key);
    void teclaJ1(unsigned char key);
    void teclaEspecial(int key);
    void teclaEspecialArriba(int key);
    void raton(int boton, int estadoRat, int x, int y);
};
