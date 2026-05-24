#pragma once
#include "posicion.h"

namespace ETSIDI { class Sprite; }

class Jugador {
protected:
    ETSIDI::Sprite* sprite{ nullptr };
    int   equipo{ 1 };
    float radio{ 0.4f };

    Jugador(float x, float y, int _equipo, const char* ruta);

public:
    Posicion pos;

    virtual ~Jugador();

    virtual void dibuja() const;
    virtual void mover(float dirX, float dirY);

    virtual int  getRango()   const = 0;
    virtual bool esVolador()  const = 0;
    virtual bool esTeleport() const = 0;

    int getEquipo() const { return equipo; }
};
