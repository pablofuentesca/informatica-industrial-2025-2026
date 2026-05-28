#pragma once

class Obstaculo {
public:
    double x, y;
    double ancho, alto;
    bool charcobool;         // false=barrera, true=charco de barro
    bool activo;
    double anchoVisual, altoVisual;
    double tAparecer;
    double tVida;

    Obstaculo() : x(0), y(0), ancho(40), alto(30), charcobool(false),
                  activo(false), anchoVisual(40), altoVisual(30),
                  tAparecer(0), tVida(0) {}

    void actualiza(double tBatalla, double dt);
    void dibuja() const;
    void separaJugador(double& px, double& py, double radio) const;
};
