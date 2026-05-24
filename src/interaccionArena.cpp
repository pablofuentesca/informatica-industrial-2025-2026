#include "interaccionArena.h"
#include "Arena.h"
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
