#pragma once
#include "pantalla.h"
#include "campotexto.h"

// Pantalla de introduccion de nombres. Si modoIA solo pide el nombre de Madrid;
// en 1vs1 pide primero el de Madrid y luego el del Atletico.
class PantallaNombres : public Pantalla {
    CampoTexto campo;
    int campoNombre{ 0 };   // 0 = Madrid, 1 = Atleti

public:
    void inicializa() override;
    void dibuja()    const override;
    void tecla(unsigned char key) override;
};
