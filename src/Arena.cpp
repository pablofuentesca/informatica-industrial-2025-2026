#include "Arena.h"
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

    glLineWidth(1.0f);
    glPointSize(1.0f);
}
