#include "ia.h"
#include "mundo.h"
#include "jugador.h"
#include "entrenador.h"
#include <iostream>
#include <climits>
#include <cmath>

int IA::valorPieza(Jugador* pj) const {
    if (pj->esEntrenador()) return 100;
    if (pj->esTeleport()) return 15;
    if (pj->esVolador()) return 8;
    return pj->getRango()*3;
}

int IA::puntuar(Jugador* pj, int dx, int dy, Jugador* capturada) const {
    int score = 0;

    if (capturada) {
        if (capturada->getEquipo() == equipo) return -1000;
        int mult = capturada->esEntrenador() ? 5 : 20;
        score += valorPieza(capturada) * mult;
    }

    int puntosX[5] = {4, 0, 8, 4, 4};
    int puntosY[5] = {4, 4, 4, 0, 8};

    for (int i = 0; i < 5; i++) {
        if (dx == puntosX[i] && dy == puntosY[i]) { score += 50; break; }
    }
    for (int i = 0; i < 5; i++) {
        int dif = abs(dx - puntosX[i]) + abs(dy - puntosY[i]);
        score += 10/(1+dif);
    }

    score += (8-dx)*2;
    score += (4 - abs(dy-4));

    return score;
}

std::vector<MovimientoIA> IA::movimientosLegales() const {
    std::vector<MovimientoIA> resultado;
    if (mundo == nullptr) return resultado;

    const std::vector<Jugador*>& piezas = mundo->equipoPorId(equipo).getPiezas();

    for (Jugador* pj : piezas) {
        if (pj == nullptr || pj->estaEncarcelado()) continue;

        int gx = (int)pj->getPosX();
        int gy = (int)pj->getPosY();
        int rango = pj->getRango();

        if (pj->esTeleport()) {
            for (int i = 0; i < 9; i++)
                for (int j = 0; j < 9; j++) {
                    if (i == gx && j == gy) continue;
                    int eq = mundo->equipoEn(i, j);
                    if (eq == equipo) continue;
                    Jugador* cap = (eq != 0) ? mundo->equipoPorId(eq).piezaEn(i, j) : nullptr;
                    resultado.emplace_back(pj, i, j, cap);
                }
        }
        else if (pj->esVolador()) {
            int dirs[8][2] = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
            for (auto& d : dirs) {
                for (int r = 1; r <= rango; r++) {
                    int nx = gx + d[0]*r;
                    int ny = gy + d[1]*r;
                    if (nx < 0 || nx >= 9 || ny < 0 || ny >= 9) break;
                    int eq = mundo->equipoEn(nx, ny);
                    if (eq == equipo) break;
                    Jugador* cap = (eq != 0) ? mundo->equipoPorId(eq).piezaEn(nx, ny) : nullptr;
                    resultado.emplace_back(pj, nx, ny, cap);
                    if (eq != 0) break;
                }
            }
        }
        else {
            int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
            for (int d = 0; d < 4; d++) {
                for (int r = 1; r <= rango; r++) {
                    int nx = gx + dirs[d][0]*r;
                    int ny = gy + dirs[d][1]*r;
                    if (nx < 0 || nx >= 9 || ny < 0 || ny >= 9) break;
                    int eq = mundo->equipoEn(nx, ny);
                    if (eq == equipo) break;
                    Jugador* cap = (eq != 0) ? mundo->equipoPorId(eq).piezaEn(nx, ny) : nullptr;
                    resultado.emplace_back(pj, nx, ny, cap);
                    if (eq != 0) break;
                }
            }
        }
    }
    return resultado;
}

MovimientoIA IA::mejorMovimiento() const {
    std::vector<MovimientoIA> opciones = movimientosLegales();

    if (opciones.empty()) {
        std::cout << "[IA] Sin movimientos disponibles.\n";
        return MovimientoIA();
    }

    MovimientoIA mejor = opciones[0];
    int mejorScore = -100000000;

    for (const MovimientoIA& m : opciones) {
        int score = puntuar(m.pieza, m.destX, m.destY, m.capturada);
        if (score > mejorScore) {
            mejorScore = score;
            mejor = m;
            mejor.score = score;
        }
    }
    return mejor;
}

void IA::puntuarConjuros(int& mejorScore, int& mejorIdx) const {
    mejorScore = -1;
    mejorIdx = -1;

    Jugador* pjEnt = mundo->equipoPorId(equipo).buscaEntrenador();
    if (pjEnt == nullptr) return;
    Entrenador* ent = static_cast<Entrenador*>(pjEnt);
    if (!ent->tieneConjuros()) return;

    int rival = (equipo == 1) ? 2 : 1;

    if (!ent->conjuroUsado(6)) {
        for (Jugador* pj : mundo->equipoPorId(rival).getPiezas())
            if (pj != nullptr && !pj->estaEncarcelado() && !pj->esEntrenador()) {
                if (100 > mejorScore) { mejorScore = 100; mejorIdx = 6; }
                break;
            }
    }

    if (!ent->conjuroUsado(5) && mundo->equipoPorId(equipo).ultimaEnCementerio() != nullptr)
        if (160 > mejorScore) { mejorScore = 160; mejorIdx = 5; }

    if (!ent->conjuroUsado(4) && 80 > mejorScore) { mejorScore = 80; mejorIdx = 4; }

    if (!ent->conjuroUsado(1)) {
        int danio = 0;
        for (Jugador* pj : mundo->equipoPorId(equipo).getPiezas())
            if (pj != nullptr) danio += pj->getHpMax() - pj->getHp();
        int s = danio*2;
        if (s > mejorScore) { mejorScore = s; mejorIdx = 1; }
    }

    int otros[] = {0, 2, 3};
    for (int i : otros)
        if (!ent->conjuroUsado(i) && 40 > mejorScore) { mejorScore = 40; mejorIdx = i; }
}

void IA::elegirMovimiento() {
    if (mundo == nullptr) return;
    if (mundo->turno.equipoEnJuego() != equipo) return;
    if (mundo->pendientePj1 != nullptr) return;
    if (fase != Fase::INACTIVA) return;

    movPendiente = mejorMovimiento();

    int conjScore, conjIdx;
    puntuarConjuros(conjScore, conjIdx);

    if (conjScore > movPendiente.score && conjIdx >= 0) {
        usarConjuro = true;
        indiceConjuro = conjIdx;
        mundo->jugadorSeleccionado = mundo->equipoPorId(equipo).buscaEntrenador();
    }
    else {
        if (movPendiente.pieza == nullptr) return;
        usarConjuro = false;
        mundo->jugadorSeleccionado = movPendiente.pieza;
    }

    mundo->calcularCasillasValidas();
    fase = Fase::SELECCIONANDO;
    timer = DELAY;
}

bool IA::mueve(double dt) {
    if (fase != Fase::SELECCIONANDO) return false;

    timer -= dt;
    if (timer > 0.0) return false;

    mundo->jugadorSeleccionado = nullptr;
    mundo->calcularCasillasValidas();

    if (usarConjuro) {
        Jugador* pjEnt = mundo->equipoPorId(equipo).buscaEntrenador();
        if (pjEnt != nullptr) {
            Entrenador* ent = static_cast<Entrenador*>(pjEnt);
            if (ent->lanzarConjuro(indiceConjuro, *mundo))
                mundo->cierraTurno();
        }
        usarConjuro = false;
    }
    else if (movPendiente.capturada != nullptr) {
        mundo->pendientePj1 = movPendiente.pieza;
        mundo->pendientePj2 = movPendiente.capturada;
        mundo->destCombateX = movPendiente.destX;
        mundo->destCombateY = movPendiente.destY;
    }
    else {
        movPendiente.pieza->moverA((float)movPendiente.destX, (float)movPendiente.destY);
        mundo->cierraTurno();
    }

    fase = Fase::INACTIVA;
    return true;
}
