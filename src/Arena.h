#pragma once

// Obstaculo del campo de combate
struct Obstaculo {
    double x, y;
    double ancho, alto;
    bool esBanderin;    // false=barrera, true=charco de barro
    bool activo;
    double anchoVisual, altoVisual; // tamaño dibujado (puede diferir del hitbox)
    double tAparecer;   // segundos desde inicio de BATALLA en que aparece
    double tVida;       // tiempo que lleva activo (para animacion de aparicion)

    Obstaculo() : x(0), y(0), ancho(40), alto(30), esBanderin(false),
                  activo(false), anchoVisual(40), altoVisual(30),
                  tAparecer(0), tVida(0) {}
};

class Arena {
    double xMin, xMax, yMin, yMax;
    double tam;

    double j1x, j1y;   // Jugador 1 - Madrid (blanco)
    double j2x, j2y;   // Jugador 2 - Atleti  (rojo)
    double arbY;        // Posicion Y del arbitro

    double t;           // Tiempo acumulado en el estado actual (segundos)
    bool silbatoSonado;
    bool usarDiagonal;  // tipo de transicion elegido al azar

    Obstaculo obstaculos[8];    // 5 barreras + 3 charcos de barro
    double tBatalla;            // tiempo transcurrido en BATALLA

    // teclas mantenidas pulsadas
    bool j1Arr, j1Aba, j1Izq, j1Der;
    bool j2Arr, j2Aba, j2Izq, j2Der;

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
              arbY(650), t(0.0), silbatoSonado(false), usarDiagonal(true),
              tBatalla(0.0),
              j1Arr(false), j1Aba(false), j1Izq(false), j1Der(false),
              j2Arr(false), j2Aba(false), j2Izq(false), j2Der(false),
              estado(TRANSICION) {}

    void inicializa();
    void dibuja() const;
    void mueve(double dt);
    void tecla(unsigned char key);
    void teclaJ1(unsigned char key);
    void teclaEspecial(int key);
    void teclaEspecialArriba(int key);
};
