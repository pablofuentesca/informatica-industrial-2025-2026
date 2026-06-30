#pragma once
#include "pantalla.h"

// Pantalla de fin de partida
class PantallaFin : public Pantalla {
public:
    void dibuja() const override;
    void tecla(unsigned char key) override;
};
