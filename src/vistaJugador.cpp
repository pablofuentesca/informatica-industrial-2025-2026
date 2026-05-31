#include "vistaJugador.h"
#include "ETSIDI.h"
#include "freeglut.h"

VistaJugador::VistaJugador(const char* ruta, float posX, float posY, float radio : sprite(new ETSIDI::Sprite(ruta, posX, posY, radio * 2.0f, radio * 2.0f)){}
VistaJugador::~VistaJugador()
{
    delete sprite;
}
void VistaJugador::dibuja(float posX, float posY) const
{
    if (sprite == nullptr) return;
    sprite->setPos(posX, posY);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    sprite->draw();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}
