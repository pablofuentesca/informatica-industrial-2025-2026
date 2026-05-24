#include "coordinador.h"
#include "ETSIDI.h"
#include "freeglut.h"

void dibujaTexto(float x, float y, const char* texto, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    while (*texto) { glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *texto); texto++; }
}

void Coordinador::mueve(double dt)
{
    if (estado == COMBATE)
        arena.mueve(dt);
}

void Coordinador::dibuja() const
{
    switch (estado) {
    case INICIO:
        glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600);
        glMatrixMode(GL_MODELVIEW);  glLoadIdentity();
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glClearColor(0.05f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        dibujaTexto(235, 400, "REAL MADRID VS ATLETICO DE MADRID", 1.0f, 1.0f, 1.0f);
        dibujaTexto(330, 350, "EDICION ARCHON",                    0.8f, 1.0f, 0.6f);
        dibujaTexto(190, 200, "Pulsa 'E' para empezar el partido", 0.6f, 0.8f, 1.0f);
        break;
    case JUEGO:
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(-1.0, 10.0, -1.0, 10.0);
        glMatrixMode(GL_MODELVIEW);  glLoadIdentity();
        mundo.dibuja();
        break;
    case COMBATE:
        glClear(GL_COLOR_BUFFER_BIT);
        arena.dibuja();
        break;
    }
}

void Coordinador::tecla(unsigned char key)
{
    switch (estado) {
    case INICIO:
        if (key == 'e' || key == 'E') { estado = JUEGO; }
        break;
    case JUEGO:
        if (key == '0') { arena.inicializa(); estado = COMBATE; return; }
        mundo.tecla(key);
        break;
    case COMBATE:
        if (key == 27) { ETSIDI::stopMusica(); estado = JUEGO; return; }  // ESC vuelve al tablero
        arena.tecla(key);
        break;
    }
}

void Coordinador::teclaJ1(unsigned char key)
{
    if (estado == COMBATE) arena.teclaJ1(key);
}

void Coordinador::teclaEspecial(int key)
{
    switch (estado) {
    case JUEGO:   mundo.teclaEspecial(key); break;
    case COMBATE: arena.teclaEspecial(key); break;
    default:      break;
    }
}

void Coordinador::teclaEspecialArriba(int key)
{
    if (estado == COMBATE) arena.teclaEspecialArriba(key);
}
