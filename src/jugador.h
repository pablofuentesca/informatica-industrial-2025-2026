#pragma once
#include "posicion.h"

namespace ETSIDI { class Sprite; }

class Jugador {
protected:
    ETSIDI::Sprite* sprite{ nullptr };
    int    equipo{ 1 };
    float  radio{ 0.4f };

    // estadisticas de combate en arena
    int    hpMax{ 50 };
    int    hp{ 50 };
    double velArena{ 200.0 };   // pixeles por segundo en la arena
    int    danio{ 10 };         // puntos de dano por ataque
    double cooldownMax{ 0.8 };  // segundos entre ataques
    double timerAtaque{ 0.0 };  // tiempo restante hasta poder atacar de nuevo

    Jugador(float x, float y, int _equipo, const char* ruta);

public:
    Posicion pos;

    virtual ~Jugador();

    virtual void dibuja() const;
    virtual void mover(float dirX, float dirY);

    // actualizacion por frame: cooldown + efectos pasivos (regen, etc.)
    virtual void actualiza(double dt);

    // tablero
    virtual int  getRango()   const = 0;
    virtual bool esVolador()  const = 0;
    virtual bool esTeleport() const = 0;

    // combate
    virtual bool   esRanged()      const = 0;
    virtual double alcanceAtaque() const;  // radio hitbox mele en px arena; 0 si es ranged
    virtual void   habilidadEspecial();    // efecto especial pasivo o activo de la pieza

    void recibeGolpe(int dano);
    void reiniciaCooldown() { timerAtaque = cooldownMax; }

    int    getHp()          const { return hp; }
    int    getHpMax()       const { return hpMax; }
    bool   estaVivo()       const { return hp > 0; }
    double getVelArena()    const { return velArena; }
    int    getDanio()       const { return danio; }
    double getCooldownMax() const { return cooldownMax; }
    bool   puedeAtacar()    const { return timerAtaque <= 0.0; }

    int getEquipo() const { return equipo; }
};
