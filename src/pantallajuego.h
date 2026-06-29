#pragma once
#include "pantalla.h"

// Pantalla del tablero principal. Delega todo en Mundo e IA.
// Cuando el mundo detecta un combate pendiente (click sobre pieza rival)
// inicializa la arena y pide pasar a COMBATE.
class PantallaJuego : public Pantalla {
public:
    void dibuja()  const override;
    void mueve(double dt) override;
    void tecla(unsigned char key) override;
    void teclaEspecial(int key)  override;
    void raton(int boton, int estado, float x, float y) override;
};
