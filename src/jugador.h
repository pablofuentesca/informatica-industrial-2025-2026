#pragma once
#include "posicion.h"

namespace ETSIDI { class Sprite; }

class Jugador {
protected:
    ETSIDI::Sprite* sprite{ nullptr };
    int    equipo{ 1 };
    float  radio{ 0.4f };
    const char* rutaTextura{ "" };  // ruta de la imagen, para dibujar en la arena

    // estadisticas de combate en arena
    int    hpMax{ 50 };
    int    hp{ 50 };
    double velArena{ 200.0 };   // pixeles por segundo en la arena
    int    danio{ 10 };         // puntos de dano por ataque
    double cooldownMax{ 0.8 };  // segundos entre ataques
    double timerAtaque{ 0.0 };  // tiempo restante hasta poder atacar de nuevo

    // flags de comportamiento especial del proyectil (se fijan en cada subclase)
    bool disparaTriple{ false };       // Manticora: dispara 3 proyectiles en abanico
    bool proyectilParaliza{ false };   // Basilisco: inmoviliza al rival al impactar
    bool proyectilAtraviesa{ false };  // Banshee: el proyectil atraviesa obstaculos
    bool disparaRayoArcano{ false };   // Entrenador: rayo rapido que atraviesa Y paraliza
    bool disparaEnjambre{ false };     // Goblin: enjambre de 3 proyectiles en abanico

    bool encarcelado{ false };
    int  ciclosEncarcelado{ 0 };

    bool temporal{ false };   // elemental invocado: vive unos turnos y desaparece
    int  ciclosVida{ 0 };

    Jugador(float x, float y, int _equipo, const char* ruta);

    Posicion pos;

public:
    virtual ~Jugador();
    Jugador(const Jugador&) = delete;//constructor de copia
    Jugador& operator=(const Jugador&) = delete;

    virtual void dibuja() const;
    virtual void mover(float dirX, float dirY);
    virtual void moverA(float x, float y);

    // actualizacion por frame: cooldown + efectos pasivos (regen, etc.)
    virtual void actualiza(double dt);

    // combate
    virtual bool   esRanged()      const = 0;
    virtual double alcanceAtaque() const;  // radio hitbox mele en px arena; 0 si es ranged
    virtual void   habilidadEspecial();    // efecto especial pasivo o activo de la pieza

    // tablero
    virtual int  getRango() const = 0;
    virtual bool esVolador() const = 0;
    virtual bool esTeleport() const = 0;
    virtual bool esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const;

    // Fenix: puede revivir una vez; el resto de piezas devuelven false
    virtual bool puedeRevivir() const { return false; }
    virtual void revive() {}

    void recibeGolpe(int dano);
    void cura(int cantidad)      { hp += cantidad; if (hp > hpMax) hp = hpMax; }
    void curarCompleto()         { hp = hpMax; }
    void reiniciaCooldown()      { timerAtaque = cooldownMax; }

    bool estaEncarcelado()       const { return encarcelado; }
    void encarcelar(int ciclos)        { encarcelado = true; ciclosEncarcelado = ciclos; }
    void descuentaCicloEncarcelado()   { if (encarcelado && --ciclosEncarcelado <= 0) encarcelado = false; }

    bool esTemporal()            const { return temporal; }
    void haceTemporal(int ciclos)      { temporal = true; ciclosVida = ciclos; }
    // descuenta un turno de vida; devuelve true cuando se agota (hay que retirarla)
    bool expiraCicloVida()             { return temporal && --ciclosVida <= 0; }

    virtual bool esEntrenador() const { return false; }

    int    getHp()          const { return hp; }
    int    getHpMax()       const { return hpMax; }
    bool   estaVivo()       const { return hp > 0; }
    double getVelArena()    const { return velArena; }
    int    getDanio()       const { return danio; }
    double getCooldownMax() const { return cooldownMax; }
    bool   puedeAtacar()    const { return timerAtaque <= 0.0; }

    int getEquipo() const { return equipo; }
    float getPosX() const;
    float getPosY() const;
    void setPosicion(float nuevaX, float nuevaY);

    bool getDisparaTriple()      const { return disparaTriple; }
    bool getProyectilParaliza()  const { return proyectilParaliza; }
    bool getProyectilAtraviesa() const { return proyectilAtraviesa; }
    bool getDisparaRayoArcano()  const { return disparaRayoArcano; }
    bool getDisparaEnjambre()    const { return disparaEnjambre; }

    const char* getRutaTextura() const { return rutaTextura; }
};
