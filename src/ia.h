#pragma once
#include <vector>
class Mundo;
class Jugador;

struct MovimientoIA {
    Jugador* pieza = nullptr;
    int destX = 0;
    int destY = 0;
    Jugador* capturada = nullptr;
    int score = -100000000;

    MovimientoIA() = default;
    MovimientoIA(Jugador* p, int x, int y, Jugador* cap = nullptr)
        : pieza(p), destX(x), destY(y), capturada(cap) {}
};

class IA {
    Mundo* mundo;
    int equipo;

    enum class Fase { INACTIVA, SELECCIONANDO };
    Fase fase{ Fase::INACTIVA };
    double timer{ 0.0 };
    MovimientoIA movPendiente;
    bool usarConjuro{ false };
    int  indiceConjuro{ -1 };
    static constexpr double DELAY = 0.7;

    int valorPieza(Jugador* pj) const;
    int puntuar(Jugador* pj, int dx, int dy, Jugador* capturada) const;
    std::vector<MovimientoIA> movimientosLegales() const;
    void puntuarConjuros(int& mejorScore, int& mejorIdx) const;

public:
    IA() : mundo(nullptr), equipo(2) {}
    void setMundo(Mundo* m) { mundo = m; }
    void setEquipo(int eq) { equipo = eq; }
    MovimientoIA mejorMovimiento() const;
    void elegirMovimiento();
    bool mueve(double dt);
};
