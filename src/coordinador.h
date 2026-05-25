#pragma once
#include "mundo.h"
#include "Arena.h"

namespace ETSIDI { class Sprite; }

class Coordinador {
    enum Estado { INICIO, JUEGO, COMBATE, PAUSA, FIN } estado{};
    Mundo mundo;
    Arena arena;
    ETSIDI::Sprite* portada{ nullptr };

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
