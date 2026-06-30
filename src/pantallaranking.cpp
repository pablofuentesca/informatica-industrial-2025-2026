#include "pantallaranking.h"
#include "contexto.h"
#include "freeglut.h"

void PantallaRanking::dibuja() const
{
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);  glLoadIdentity();
    glDisable(GL_LIGHTING);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ctx->ranking.dibuja();  
}

void PantallaRanking::tecla(unsigned char key)
{
    if (key == 27) ctx->pide(Contexto::INICIO); 
}
