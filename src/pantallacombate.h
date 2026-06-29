#pragma once
#include "pantalla.h"

// Pantalla de la arena de combate. Hace avanzar la batalla y la dibuja; cuando
// hay un ganador (o el jugador pulsa ESC para abandonar) resuelve el combate en
// el tablero y pide volver al tablero (JUEGO) o a la pantalla final (FIN).
// La arena ya debe venir inicializada con los dos combatientes desde el tablero.
class PantallaCombate : public Pantalla {
public:
    void dibuja() const override;
    void mueve(double dt) override;
    void tecla(unsigned char key) override;
    void teclaArriba(unsigned char key) override;
    void teclaEspecial(int key) override;
    void teclaEspecialArriba(int key) override;
};
