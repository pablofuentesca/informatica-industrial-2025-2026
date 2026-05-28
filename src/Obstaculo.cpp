#include "Obstaculo.h"
#include "ETSIDI.h"
#include "freeglut.h"

static double limitacionRangoX(double x) {
    if (x < 0.0) return 0.0;
    if (x > 1.0) return 1.0;
    return x;
}

void Obstaculo::actualiza(double tBatalla, double dt)
{
    if (!activo && tBatalla >= tAparecer)
        activo = true;
    if (activo)
        tVida += dt;
}

void Obstaculo::separaJugador(double& px, double& py, double radio) const
{
    double dx = px - x;  if (dx < 0) dx = -dx;
    double dy = py - y;  if (dy < 0) dy = -dy;
    double overlapX = (radio + ancho / 2.0) - dx;
    double overlapY = (radio + alto  / 2.0) - dy;
    if (overlapX > 0 && overlapY > 0) {
        if (overlapX < overlapY)
            px += (px > x ? overlapX : -overlapX);
        else
            py += (py > y ? overlapY : -overlapY);
    }
}

void Obstaculo::dibuja() const
{
    if (!activo) return;

    static ETSIDI::GLTexture texBarrera = ETSIDI::getTexture("imagenes/barrera.png");
    static ETSIDI::GLTexture texBarro   = ETSIDI::getTexture("imagenes/barro.png");

    double escala = limitacionRangoX(tVida / 0.3);
    double a2 = anchoVisual * escala / 2.0;
    double b2 = altoVisual  * escala / 2.0;

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (charcobool) {
        glBindTexture(GL_TEXTURE_2D, texBarro.id);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glColor3d(1.0, 1.0, 1.0);
        glBegin(GL_QUADS);
            glTexCoord2d(0, 0); glVertex2d(x - a2, y - b2);
            glTexCoord2d(1, 0); glVertex2d(x + a2, y - b2);
            glTexCoord2d(1, 1); glVertex2d(x + a2, y + b2);
            glTexCoord2d(0, 1); glVertex2d(x - a2, y + b2);
        glEnd();
    } else {
        glBindTexture(GL_TEXTURE_2D, texBarrera.id);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glBegin(GL_QUADS);
            glTexCoord2d(0, 1); glVertex2d(x - a2, y - b2);
            glTexCoord2d(1, 1); glVertex2d(x + a2, y - b2);
            glTexCoord2d(1, 0); glVertex2d(x + a2, y + b2);
            glTexCoord2d(0, 0); glVertex2d(x - a2, y + b2);
        glEnd();
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}
