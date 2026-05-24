#pragma once
#include "mundo.h"
#include "Arena.h"

class Coordinador {
    enum Estado { INICIO, JUEGO, COMBATE, PAUSA, FIN } estado{};
    Mundo mundo;
    Arena arena;

public:
    void dibuja() const;
    void tecla(unsigned char key);
    void teclaEspecial(int key);
};
