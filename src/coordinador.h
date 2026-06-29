#pragma once
#include "contexto.h"

class Pantalla;

// El coordinador ya no sabe de logica de juego, solo guarda el contexto
// y delega todo en la pantalla que este activa en cada momento.
class Coordinador {
    Contexto ctx;
    Pantalla* actual{ nullptr };

    void cambiaPantalla(Contexto::Id id);
    void procesaCambio();

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
