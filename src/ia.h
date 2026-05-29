#pragma once
#include <vector>
class Mundo;
class Jugador;

struct MovimientoIA {
    Jugador* pieza = nullptr;
    int destX = 0;
    int destY = 0;
    Jugador* capturada = nullptr;

    MovimientoIA() = default;
    MovimientoIA(Jugador* p, int x, int y, Jugador* cap = nullptr)
        : pieza(p), destX(x), destY(y), capturada(cap) {}
};

class IA {
    Mundo* mundo;
    int equipo;

    int valorPieza(Jugador* pj) const;
    int puntuar(Jugador* pj, int dx, int dy, Jugador* capturada) const;
    std::vector<MovimientoIA> movimientosLegales() const;

public:
    IA() : mundo(nullptr), equipo(2) {}
    void setMundo(Mundo* m) { mundo = m; }
    void setEquipo(int eq) { equipo = eq; }
    MovimientoIA mejorMovimiento() const;
    void elegirMovimiento();
};
