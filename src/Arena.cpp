#include "Arena.h"
#include "freeglut.h"

void Arena::dibuja() const
{
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);  glLoadIdentity();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

    // Fondo verde del campo de combate
    glColor3d(0.1, 0.6, 0.1);
    glBegin(GL_QUADS);
        glVertex2d(xMin, yMin);
        glVertex2d(xMax, yMin);
        glVertex2d(xMax, yMax);
        glVertex2d(xMin, yMax);
    glEnd();
}
