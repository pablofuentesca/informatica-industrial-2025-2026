#pragma once
#include "mundo.h"

// Los estados por los que puede pasar nuestra aplicación
enum Estado { INICIO, JUEGO, PAUSA, FIN };

class Coordinador {
protected:
    Mundo mundo;
    Estado estado;

public:
    Coordinador();
    ~Coordinador();

    void dibuja();
    void tecla(unsigned char key);
    void teclaEspecial(int key);
};