#include "boton.h"
#include "freeglut.h"

void Boton::dibuja() const
{
    // Fondo translucido
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.05f, 0.03f, 0.0f, 0.85f);
    glBegin(GL_QUADS);
        glVertex2f(x1, y1); glVertex2f(x2, y1);
        glVertex2f(x2, y2); glVertex2f(x1, y2);
    glEnd();
    glDisable(GL_BLEND);

    // Borde
    glLineWidth(2.5f);
    glColor3f(0.9f, 0.7f, 0.1f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x1, y1); glVertex2f(x2, y1);
        glVertex2f(x2, y2); glVertex2f(x1, y2);
    glEnd();
    glLineWidth(1.0f);

    // Texto
    float textW = (float)glutBitmapLength(GLUT_BITMAP_HELVETICA_18,(const unsigned char*)texto);
    float cx = x1 + (x2 - x1 - textW) / 2.0f;
    float cy = y1 + (y2 - y1) / 2.0f - 6.0f;
    glColor3f(1.0f, 0.85f, 0.4f);
    glRasterPos2f(cx, cy);
    for (const char* c = texto; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}
