#include "pantallafin.h"
#include "contexto.h"
#include "freeglut.h"
#include <string>

static void dibujaTexto(float x, float y, const char* texto, float r, float g, float b)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    while (*texto) { glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *texto); texto++; }
}

void PantallaFin::dibuja() const
{
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);  glLoadIdentity();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    std::string ganador = "GANA " + (ctx->equipoVencedor == 1 ? ctx->nombreMadrid : ctx->nombreAtleti);
    dibujaTexto(250.0f, 330.0f, ganador.c_str(), 1.0f, 0.85f, 0.1f);
    dibujaTexto(270.0f, 285.0f, "Pulsa ESC para volver al inicio", 0.6f, 0.6f, 0.6f);
    dibujaTexto(270.0f, 255.0f, "Pulsa C para ver el ranking", 0.6f, 0.6f, 0.6f);
}

void PantallaFin::tecla(unsigned char key)
{
    if (key == 27) { ctx->equipoVencedor = 0; ctx->pide(Contexto::INICIO); }
    if (key == 'c' || key == 'C') ctx->pide(Contexto::RANKING);
}
