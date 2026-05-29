#pragma once
#include "teletransportador.h"
#include "conjuro.h"
#include <vector>
class Mundo;

// Equipo 1 (Madrid):  Mago      (Ancelotti) — PIEZA REY, ranged, 7 conjuros por partida
// Equipo 2 (Atleti):  Hechicera (Simeone)   — PIEZA REY, ranged, 7 conjuros por partida
// Si el Entrenador es eliminado en arena, el partido termina para ese bando
class Entrenador : public Teletransportador {
    int conjurosRestantes{ 7 };
    std::vector<Conjuro*> conjuros;

public:
    Entrenador(float x, float y, int equipo)
        : Teletransportador(x, y, equipo, equipo == 1
            ? "../bin/imagenes/fotosjugadores/madridentrenador.png"
            : "../bin/imagenes/fotosjugadores/atletientrenador.png")
    {
        hpMax             = 40;
        velArena          = 130.0;
        danio             = 12;
        cooldownMax       = 1.00;
        disparaRayoArcano = true;
        hp = hpMax;

        conjuros.push_back(new ConjuroTeleport());
        conjuros.push_back(new ConjuroCurar());
        conjuros.push_back(new ConjuroCambioCiclo());
        conjuros.push_back(new ConjuroIntercambiar());
        conjuros.push_back(new ConjuroInvocar());
        conjuros.push_back(new ConjuroRevivir());
        conjuros.push_back(new ConjuroEncarcelar());
    }

    ~Entrenador() override {
        for (Conjuro* c : conjuros) delete c;
    }

    // En el tablero puede teletransportarse a cualquier casilla libre
    int getRango() const override { return 3; }

    bool   esRanged()      const override { return true; }
    double alcanceAtaque() const override { return 0.0; }

    void mover(float dirX, float dirY) override {
        Jugador::mover(dirX, dirY);
    }

    int  getConjurosRestantes() const { return conjurosRestantes; }
    bool tieneConjuros()        const { return conjurosRestantes > 0; }

    bool gastaConjuro() {
        if (conjurosRestantes <= 0) return false;
        conjurosRestantes--;
        return true;
    }
    int         numConjuros()       const { return (int)conjuros.size(); }
    const char* nombreConjuro(int i) const {
        if (i < 0 || i >= (int)conjuros.size()) return "";
        return conjuros[i]->nombre();
    }
    bool conjuroUsado(int i) const {
        if (i < 0 || i >= (int)conjuros.size()) return true;
        return conjuros[i]->estaUsado();
    }

    // lanza el conjuro i; devuelve true si tuvo efecto
    bool lanzarConjuro(int i, Mundo& m);

    bool esEntrenador() const override { return true; }
    void habilidadEspecial() override {}

    bool esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const override;
};
