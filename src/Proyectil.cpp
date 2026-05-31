#include "Proyectil.h"

Proyectil::Proyectil()
    : x(0), y(0), dx(1), dy(0), vel(350.0), danio(10),
      equipo(1), activo(false),
      atraviesaObstaculos(false), paraliza(false),
      distanciaMax(0.0), distRecorrida(0.0)
{}
