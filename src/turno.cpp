#include "turno.h"
#include <cstdlib>
#include <ctime>

Turno::Turno() : equipoActual(1), primero(true) {}

void Turno::sortea()
{
    srand((unsigned)time(nullptr));
    equipoActual = rand() % 2 + 1;
    primero      = true;
}

void Turno::cambia()
{
    primero      = false;
    equipoActual = (equipoActual == 1) ? 2 : 1;
}
