#pragma once

class Arena {
    double xMin, xMax, yMin, yMax;
    double tam;

    double j1x, j1y;   // Jugador 1 - Madrid (blanco)
    double j2x, j2y;   // Jugador 2 - Atleti  (rojo)
    double arbY;        // Posicion Y del arbitro

    double t;           // Tiempo acumulado en el estado actual (segundos)
    bool silbatoSonado;
    bool usarDiagonal;  // tipo de transicion elegido al azar

    enum EstadoArena {
        TRANSICION,
        ENTRADA,
        ARBITRO_ENTRANDO,
        ARBITRO_ESPERA,
        ARBITRO_SALIENDO,
        BATALLA
    } estado;

    friend class interaccionArena;

public:
    Arena() : xMin(50), xMax(750), yMin(50), yMax(550),
              tam(25),
              j1x(-50), j1y(300), j2x(850), j2y(300),
              arbY(650), t(0.0), silbatoSonado(false), usarDiagonal(true), estado(TRANSICION) {}

    void inicializa();
    void dibuja() const;
    void mueve(double dt);
    void tecla(unsigned char key);
    void teclaEspecial(int key);
};
