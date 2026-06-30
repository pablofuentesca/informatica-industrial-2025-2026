#pragma once
#include "pantalla.h"

// Pantalla con la tabla de victorias.
class PantallaRanking : public Pantalla {
public:
    void dibuja() const override;
    void tecla(unsigned char key) override;
};
