#include "interaccionArena.h"
#include "Arena.h"
#include "jugador.h"
#include <cmath>

void interaccionArena::separa(Arena& a)
{
    double dx      = a.j1x - a.j2x;
    double dy      = a.j1y - a.j2y;
    double minDist = 2.0 * a.tam;

    if (std::abs(dx) < minDist && std::abs(dy) < minDist) {
        double overlapX = minDist - std::abs(dx);
        double overlapY = minDist - std::abs(dy);
        double empuje   = 0.0;

        if (overlapX < overlapY) {
            empuje = overlapX / 2.0;
            if (dx >= 0) { a.j1x += empuje; a.j2x -= empuje; }
            else         { a.j1x -= empuje; a.j2x += empuje; }
        } else {
            empuje = overlapY / 2.0;
            if (dy >= 0) { a.j1y += empuje; a.j2y -= empuje; }
            else         { a.j1y -= empuje; a.j2y += empuje; }
        }

        // Reaplica límites tras la separación
        if (a.j1x - a.tam < a.xMin) a.j1x = a.xMin + a.tam;
        if (a.j1x + a.tam > a.xMax) a.j1x = a.xMax - a.tam;
        if (a.j1y - a.tam < a.yMin) a.j1y = a.yMin + a.tam;
        if (a.j1y + a.tam > a.yMax) a.j1y = a.yMax - a.tam;

        if (a.j2x - a.tam < a.xMin) a.j2x = a.xMin + a.tam;
        if (a.j2x + a.tam > a.xMax) a.j2x = a.xMax - a.tam;
        if (a.j2y - a.tam < a.yMin) a.j2y = a.yMin + a.tam;
        if (a.j2y + a.tam > a.yMax) a.j2y = a.yMax - a.tam;
    }
}

void interaccionArena::aplicaDanio(Arena& a, double dt)
{
    if (a.j1ptr == nullptr || a.j2ptr == nullptr) return;

    // actualiza cooldowns de ataque de ambas piezas
    a.j1ptr->actualiza(dt);
    a.j2ptr->actualiza(dt);

    double dx   = a.j1x - a.j2x;
    double dy   = a.j1y - a.j2y;
    double dist = std::sqrt(dx * dx + dy * dy);

    // umbral de contacto para ataque (melee y ranged simplificado)
    double rango = 2.0 * a.tam + 8.0;

    if (dist < rango) {
        // j1 ataca a j2
        if (a.j1ptr->puedeAtacar()) {
            a.j2ptr->recibeGolpe(a.j1ptr->getDanio());
            a.j1ptr->reiniciaCooldown();
            if (!a.j2ptr->estaVivo()) { a.resultado = Arena::GANA_J1; return; }
        }
        // j2 ataca a j1
        if (a.j2ptr->puedeAtacar()) {
            a.j1ptr->recibeGolpe(a.j2ptr->getDanio());
            a.j2ptr->reiniciaCooldown();
            if (!a.j1ptr->estaVivo()) { a.resultado = Arena::GANA_J2; return; }
        }
    }
}
