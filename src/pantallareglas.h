#pragma once
#include "pantalla.h"
class PantallaReglas : public Pantalla {
public:
    void dibuja() const override;
    void tecla(unsigned char key) override;
};
