#pragma once
#include "mundo.h"
#include "Arena.h"

class Coordinador {
    enum Estado { INICIO, JUEGO, COMBATE, PAUSA, FIN } estado{};
    Mundo mundo;
    Arena arena;

public:
    void inicializa() { mundo.inicializa(); }
    void dibuja() const;
    void mueve(double dt);
    void tecla(unsigned char key);
    void teclaEspecial(int key);
};
