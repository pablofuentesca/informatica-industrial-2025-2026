#pragma once
#include "volador.h"
#include "hechicero.h"
class Mundo;

// Base para las piezas Rey — no instanciar directamente
// Se mueven como reina hasta 3 casillas; disparan rayo arcano que atraviesa y paraliza
class Entrenador : public Volador, public Hechicero {
protected:
    Entrenador(float x, float y, int eq, const char* ruta): Volador(x, y, eq, ruta), Hechicero()
    {
        hpMax = 40; velArena = 130.0; danio = 12; cooldownMax = 1.00;
        disparaRayoArcano = true;
        hp = hpMax;
    }
public:
    int getRango() const override { return 3; }
    bool esRanged()  const override { return true; }
    double alcanceAtaque() const override { return 0.0; }
    bool esEntrenador()const override { return true; }
    void habilidadEspecial() override {}
    bool lanzarConjuro(int i, Mundo& m) { return Hechicero::lanzarConjuro(i, m, getEquipo()); }
    bool esMovimientoValido(int ox, int oy, int dx, int dy) const override;
};

// Mago — Madrid (equipo 1)
// Hechicero de la luz; comanda las 7 fuerzas del tablero
class Mago : public Entrenador {
public:
    Mago(float x, float y) : Entrenador(x, y, 1, "../bin/imagenes/fotosjugadores/madridentrenador.png") {}
};

// Hechicera — Atleti (equipo 2)
// Bruja de las sombras; domina las mismas 7 fuerzas desde la oscuridad
class Hechicera : public Entrenador {
public:
    Hechicera(float x, float y) : Entrenador(x, y, 2, "../bin/imagenes/fotosjugadores/atletientrenador.png") {}
};
