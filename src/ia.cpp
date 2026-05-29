#include "ia.h"
#include "mundo.h"
#include "jugador.h"
#include <iostream>
#include <climits>
#include <cmath>

int IA::valorPieza(Jugador* pj) const {
    if (pj->esEntrenador()) return 100;
    if (pj->esTeleport()) return 15;
    if (pj->esVolador()) return 8;
    return pj->getRango() * 3;
}

int IA::puntuar(Jugador* pj, int dx, int dy, Jugador* capturada) const {
    int score = 0;

    if (capturada) {
        if (capturada->getEquipo() == equipo) return -1000;
        score += valorPieza(capturada) * 20;
    }

    int puntosX[5] = {4, 0, 8, 4, 4};
    int puntosY[5] = {4, 4, 4, 0, 8};
    for (int i = 0; i < 5; i++) {
        if (dx == puntosX[i] && dy == puntosY[i]) { score += 50; break; }
    }

    for (int i = 0; i < 5; i++) {
        int dif = abs(dx - puntosX[i]) + abs(dy - puntosY[i]);
        score += 10 / (1 + dif);
    }

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
        }
    }
    return mejor;
}

void IA::elegirMovimiento() {
    if (mundo == nullptr) return;
    if (mundo->turno.equipoEnJuego() != equipo) return;
    if (mundo->pendientePj1 != nullptr) return;

    MovimientoIA m = mejorMovimiento();
    if (m.pieza == nullptr) return;

    if (m.capturada != nullptr) {
        mundo->pendientePj1 = m.pieza;
        mundo->pendientePj2 = m.capturada;
        mundo->destCombateX = m.destX;
        mundo->destCombateY = m.destY;
    }
    else {
        m.pieza->moverA((float)m.destX, (float)m.destY);
        mundo->cierraTurno();
    }
}
