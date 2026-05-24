#include "jugador.h"
#include "ETSIDI.h"
#include "freeglut.h"

Jugador::Jugador(float x, float y, int _equipo, const char* ruta)
{
    pos.x  = x + 0.5f;
    pos.y  = y + 0.5f;
    equipo = _equipo;
    sprite = new ETSIDI::Sprite(ruta, pos.x, pos.y, radio * 2, radio * 2);
}

Jugador::~Jugador()
{
    if (sprite != nullptr) delete sprite;
}

void Jugador::dibuja() const
{
    if (sprite != nullptr) {
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        sprite->draw();
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    }
}

void Jugador::mover(float dirX, float dirY)
{
    float nuevaX = pos.x + dirX;
    float nuevaY = pos.y + dirY;

    if (nuevaX > 0.0f && nuevaX < 9.0f && nuevaY > 0.0f && nuevaY < 9.0f) {
        pos.x = nuevaX;
        pos.y = nuevaY;
        if (sprite != nullptr) sprite->setPos(pos.x, pos.y);
    }
}
