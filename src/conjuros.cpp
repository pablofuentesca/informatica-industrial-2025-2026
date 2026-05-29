#include "conjuro.h"
#include "mundo.h"

bool ConjuroTeleport::aplicar(Mundo& m, int equipo)
{
    if (!m.teleportarAleatoriamente(equipo)) return false;
    usado = true;
    return true;
}

bool ConjuroCurar::aplicar(Mundo& m, int equipo)
{
    if (!m.curarUnaPieza(equipo)) return false;
    usado = true;
    return true;
}

bool ConjuroCambioCiclo::aplicar(Mundo& m, int)
{
    m.invertirCiclo();
    usado = true;
    return true;
}

bool ConjuroIntercambiar::aplicar(Mundo& m, int equipo)
{
    if (!m.intercambiarPiezas(equipo)) return false;
    usado = true;
    return true;
}

bool ConjuroInvocar::aplicar(Mundo& m, int equipo)
{
    if (!m.invocarElemental(equipo)) return false;
    usado = true;
    return true;
}

bool ConjuroRevivir::aplicar(Mundo& m, int equipo)
{
    if (!m.revivirPieza(equipo)) return false;
    usado = true;
    return true;
}

bool ConjuroEncarcelar::aplicar(Mundo& m, int equipo)
{
    if (!m.encarcelarEnemigo(equipo)) return false;
    usado = true;
    return true;
}
