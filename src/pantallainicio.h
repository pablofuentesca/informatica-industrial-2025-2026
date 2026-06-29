#pragma once
#include "pantalla.h"
#include "boton.h"

namespace ETSIDI { class Sprite; }

// Pantalla del menu principal: muestra la portada, los dos botones de modo de
// juego y los atajos de teclado. Al pulsar un boton o la tecla E fija modoIA
// y pide pasar a NOMBRES; R va a REGLAS y C al RANKING.
class PantallaInicio : public Pantalla {
    ETSIDI::Sprite* portada{ nullptr };
    Boton botones[2];

public:
    ~PantallaInicio() override;
    void inicializa() override;
    void dibuja() const override;
    void tecla(unsigned char key) override;
    void raton(int boton, int estado, float x, float y) override;
};
