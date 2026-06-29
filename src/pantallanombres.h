#pragma once
#include "pantalla.h"

// Pantalla donde los jugadores escriben su nombre antes de empezar.
class PantallaNombres : public Pantalla {
public:
    explicit PantallaNombres(bool ia) {}   // de momento ignora ia
    void dibuja() const override {}
};
