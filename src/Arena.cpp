#include "Arena.h"
#include "interaccionArena.h"
#include "freeglut.h"
#include <cmath>

static const double PI = 3.14159265358979;

static void dibujaCirculo(double cx, double cy, double r, int seg = 64)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < seg; i++) {
        double a = 2.0 * PI * i / seg;
        glVertex2d(cx + r * cos(a), cy + r * sin(a));
    }
    glEnd();
}

void Arena::dibuja() const
{
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);  glLoadIdentity();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

    double cx = (xMin + xMax) / 2.0;
    double cy = (yMin + yMax) / 2.0;

    // Franjas de césped alternadas
    double anchoFranja = (xMax - xMin) / 10.0;
    for (int i = 0; i < 10; i++) {
        double x0 = xMin + i * anchoFranja;
        double x1 = x0 + anchoFranja;
        if (i % 2 == 0) glColor3d(0.13, 0.62, 0.13);
        else             glColor3d(0.10, 0.55, 0.10);
        glBegin(GL_QUADS);
            glVertex2d(x0, yMin); glVertex2d(x1, yMin);
            glVertex2d(x1, yMax); glVertex2d(x0, yMax);
        glEnd();
    }

    // Líneas blancas
    glColor3d(1.0, 1.0, 1.0);
    glLineWidth(2.0f);

    // Borde exterior
    glBegin(GL_LINE_LOOP);
        glVertex2d(xMin, yMin); glVertex2d(xMax, yMin);
        glVertex2d(xMax, yMax); glVertex2d(xMin, yMax);
    glEnd();

    // Línea de centro
    glBegin(GL_LINES);
        glVertex2d(cx, yMin); glVertex2d(cx, yMax);
    glEnd();

    // Círculo y punto central
    dibujaCirculo(cx, cy, 70.0);
    glPointSize(6.0f);
    glBegin(GL_POINTS);
        glVertex2d(cx, cy);
    glEnd();

    // Área de penalti izquierda
    glBegin(GL_LINE_LOOP);
        glVertex2d(xMin,       yMin + 110);
        glVertex2d(xMin + 160, yMin + 110);
        glVertex2d(xMin + 160, yMax - 110);
        glVertex2d(xMin,       yMax - 110);
    glEnd();

    // Área pequeña izquierda
    glBegin(GL_LINE_LOOP);
        glVertex2d(xMin,      yMin + 185);
        glVertex2d(xMin + 70, yMin + 185);
        glVertex2d(xMin + 70, yMax - 185);
        glVertex2d(xMin,      yMax - 185);
    glEnd();

    // Área de penalti derecha
    glBegin(GL_LINE_LOOP);
        glVertex2d(xMax,       yMin + 110);
        glVertex2d(xMax - 160, yMin + 110);
        glVertex2d(xMax - 160, yMax - 110);
        glVertex2d(xMax,       yMax - 110);
    glEnd();

    // Área pequeña derecha
    glBegin(GL_LINE_LOOP);
        glVertex2d(xMax,      yMin + 185);
        glVertex2d(xMax - 70, yMin + 185);
        glVertex2d(xMax - 70, yMax - 185);
        glVertex2d(xMax,      yMax - 185);
    glEnd();

    // Puntos de penalti
    glPointSize(5.0f);
    glBegin(GL_POINTS);
        glVertex2d(xMin + 110, cy);
        glVertex2d(xMax - 110, cy);
    glEnd();

    // Jugador 1 - Madrid (blanco)
    glColor3d(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        glVertex2d(j1x - tam, j1y - tam); glVertex2d(j1x + tam, j1y - tam);
        glVertex2d(j1x + tam, j1y + tam); glVertex2d(j1x - tam, j1y + tam);
    glEnd();

    // Jugador 2 - Atleti (rojo)
    glColor3d(0.9, 0.1, 0.1);
    glBegin(GL_QUADS);
        glVertex2d(j2x - tam, j2y - tam); glVertex2d(j2x + tam, j2y - tam);
        glVertex2d(j2x + tam, j2y + tam); glVertex2d(j2x - tam, j2y + tam);
    glEnd();

    glLineWidth(1.0f);
    glPointSize(1.0f);
}

void Arena::tecla(unsigned char key)
{
    const double paso = 5.0;
    if (key == 'w' || key == 'W') j1y += paso;
    if (key == 's' || key == 'S') j1y -= paso;
    if (key == 'a' || key == 'A') j1x -= paso;
    if (key == 'd' || key == 'D') j1x += paso;

    // Límites del campo para j1
    if (j1x - tam < xMin) j1x = xMin + tam;
    if (j1x + tam > xMax) j1x = xMax - tam;
    if (j1y - tam < yMin) j1y = yMin + tam;
    if (j1y + tam > yMax) j1y = yMax - tam;

    interaccionArena::separa(*this);
}

void Arena::teclaEspecial(int key)
{
    const double paso = 5.0;
    if (key == GLUT_KEY_UP)    j2y += paso;
    if (key == GLUT_KEY_DOWN)  j2y -= paso;
    if (key == GLUT_KEY_LEFT)  j2x -= paso;
    if (key == GLUT_KEY_RIGHT) j2x += paso;

    // Límites del campo para j2
    if (j2x - tam < xMin) j2x = xMin + tam;
    if (j2x + tam > xMax) j2x = xMax - tam;
    if (j2y - tam < yMin) j2y = yMin + tam;
    if (j2y + tam > yMax) j2y = yMax - tam;

    interaccionArena::separa(*this);
}
