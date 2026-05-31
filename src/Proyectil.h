#pragma once

class Proyectil {
public:
    double x, y;
    double dx, dy;              // direccion normalizada
    double vel;                 // pixeles por segundo
    int    danio;
    int    equipo;              // 1 o 2: quien lo disparo
    bool   activo;
    bool   atraviesaObstaculos; // Banshee: ignora barreras y charcos
    bool   paraliza;            // Basilisco: inmoviliza al rival 1 s
    double distanciaMax;        // max px antes de desvanecerse (0 = sin limite)
    double distRecorrida;       // acumulado desde el disparo

    Proyectil();
};
