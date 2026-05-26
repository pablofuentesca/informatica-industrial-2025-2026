#pragma once

class Jugador;   // declaracion anticipada: Arena solo guarda punteros

// Obstaculo del campo de combate
struct Obstaculo {
    double x, y;
    double ancho, alto;
    bool esBanderin;    // false=barrera, true=charco de barro
    bool activo;
    double anchoVisual, altoVisual;
    double tAparecer;
    double tVida;

    Obstaculo() : x(0), y(0), ancho(40), alto(30), esBanderin(false),
                  activo(false), anchoVisual(40), altoVisual(30),
                  tAparecer(0), tVida(0) {}
};

// Proyectil disparado en la arena
struct Proyectil {
    double x, y;
    double dx, dy;              // direccion normalizada
    double vel;                 // pixeles por segundo
    int    danio;
    int    equipo;              // 1 o 2: quien lo disparo
    bool   activo;
    bool   atraviesaObstaculos; // Banshee: ignora barreras y charcos
    bool   paraliza;            // Basilisco: inmoviliza al rival 1 s

    Proyectil() : x(0), y(0), dx(1), dy(0), vel(350.0), danio(10),
                  equipo(1), activo(false),
                  atraviesaObstaculos(false), paraliza(false) {}
};

class Arena {
    static const int MAX_PROYECTILES = 32;

    double xMin, xMax, yMin, yMax;
    double tam;

    double j1x, j1y;   // Jugador 1 - Madrid (blanco)
    double j2x, j2y;   // Jugador 2 - Atleti  (rojo)
    double arbY;

    double t;
    bool   silbatoSonado;
    bool   usarDiagonal;

    Obstaculo obstaculos[8];
    double    tBatalla;

    Proyectil proyectiles[MAX_PROYECTILES];

    Jugador* pj1;   // pieza del equipo 1 en combate (puede ser nullptr en pruebas)
    Jugador* pj2;   // pieza del equipo 2 en combate

    double timerParalizadoJ1;   // segundos restantes inmovilizado por Basilisco
    double timerParalizadoJ2;

    double timerMeleeJ1;   // duracion del destello visual del golpe mele J1
    double timerMeleeJ2;

    int ganador;    // 0=sin resultado, 1=gana Madrid, 2=gana Atleti

    // teclas mantenidas pulsadas
    bool j1Arr, j1Aba, j1Izq, j1Der;
    bool j2Arr, j2Aba, j2Izq, j2Der;
    bool j1Ataca;   // ESPACIO mantenido
    bool j2Ataca;   // INTRO mantenido

    enum EstadoArena {
        TRANSICION,
        ENTRADA,
        ARBITRO_ENTRANDO,
        ARBITRO_ESPERA,
        ARBITRO_SALIENDO,
        BATALLA,
        FIN
    } estado;

    friend class interaccionArena;

    // metodos privados de combate
    void lanzaAtaque(int equipo);
    void agregaProyectil(double ox, double oy, double dx, double dy,
                         int danio, int equipo, bool atraviesa, bool paraliza,
                         double vel = 350.0);
    void actualizaProyectiles(double dt);
    void dibujaBarrasVida() const;

public:
    Arena() : xMin(50), xMax(750), yMin(50), yMax(550),
              tam(25),
              j1x(-50), j1y(300), j2x(850), j2y(300),
              arbY(650), t(0.0), silbatoSonado(false), usarDiagonal(true),
              tBatalla(0.0),
              pj1(nullptr), pj2(nullptr),
              timerParalizadoJ1(0.0), timerParalizadoJ2(0.0),
              timerMeleeJ1(0.0), timerMeleeJ2(0.0),
              ganador(0),
              j1Arr(false), j1Aba(false), j1Izq(false), j1Der(false),
              j2Arr(false), j2Aba(false), j2Izq(false), j2Der(false),
              j1Ataca(false), j2Ataca(false),
              estado(TRANSICION) {}

    void inicializa();
    void inicializa(Jugador* combatiente1, Jugador* combatiente2);
    void dibuja() const;
    void mueve(double dt);
    void tecla(unsigned char key);
    void teclaJ1(unsigned char key);
    void teclaEspecial(int key);
    void teclaEspecialArriba(int key);

    int getGanador() const { return ganador; }
};
