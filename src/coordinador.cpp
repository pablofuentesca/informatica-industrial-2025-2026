#include "coordinador.h"
#include "ETSIDI.h"
#include "freeglut.h"

Coordinador::~Coordinador()
{
    if (portada != nullptr) delete portada;
}

void Coordinador::inicializa()
{
    portada = new ETSIDI::Sprite("../bin/imagenes/portada.png", 400, 300, 800, 600);
    mundo.inicializa();
}

static void dibujaTexto(float x, float y, const char* texto, float r, float g, float b)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    while (*texto) { glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *texto); texto++; }
}

static void dibujaBoton(float x1, float y1, float x2, float y2,
                         const char* texto, bool activo)
{
    // Fondo oscuro semitransparente
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.05f, 0.03f, 0.0f, 0.85f);
    glBegin(GL_QUADS);
        glVertex2f(x1, y1); glVertex2f(x2, y1);
        glVertex2f(x2, y2); glVertex2f(x1, y2);
    glEnd();
    glDisable(GL_BLEND);

    // Borde dorado (grisáceo si inactivo)
    glLineWidth(2.5f);
    glColor3f(activo ? 0.9f : 0.45f, activo ? 0.7f : 0.4f, activo ? 0.1f : 0.1f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x1, y1); glVertex2f(x2, y1);
        glVertex2f(x2, y2); glVertex2f(x1, y2);
    glEnd();
    glLineWidth(1.0f);

    // Texto centrado
    float textW = (float)glutBitmapLength(GLUT_BITMAP_HELVETICA_18,
                                          (const unsigned char*)texto);
    float cx = x1 + (x2 - x1 - textW) / 2.0f;
    float cy = y1 + (y2 - y1) / 2.0f - 6.0f;
    glColor3f(activo ? 1.0f : 0.5f, activo ? 0.85f : 0.5f, activo ? 0.4f : 0.4f);
    glRasterPos2f(cx, cy);
    for (const char* c = texto; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
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
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (portada != nullptr) {
            glEnable(GL_TEXTURE_2D);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            portada->draw();
            glDisable(GL_TEXTURE_2D);
        }

        // Resetear estados que el sprite puede haber dejado sucios
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        dibujaBoton(200, 120, 390, 185, "1 vs 1",       true);
        dibujaBoton(410, 120, 620, 185, "Jugador vs IA", true);
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
        if (key == 27) { ETSIDI::stopMusica(); estado = JUEGO; return; }
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

void Coordinador::raton(int boton, int estadoRat, int x, int y)
{
    if (estado == INICIO) {
        if (boton == GLUT_LEFT_BUTTON && estadoRat == GLUT_DOWN) {
            // Botón "1 vs 1": OpenGL x[200-390] y[120-185] → GLUT y[415-480]
            if (x >= 200 && x <= 390 && y >= 415 && y <= 480)
                estado = JUEGO;
        }
        return;
    }

    if (estado == JUEGO) {
        float x_logico = (x / 800.0f) * 11.0f - 1.0f;
        float y_logico = ((600.0f - y) / 600.0f) * 11.0f - 1.0f;
        mundo.raton(boton, estadoRat, x_logico, y_logico);
    }
}
