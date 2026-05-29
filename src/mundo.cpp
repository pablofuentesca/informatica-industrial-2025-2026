#include "mundo.h"
#include <cmath>
#include <algorithm>
#include "ETSIDI.h"
#include "portero.h"
#include "delantero.h"
#include "central.h"
#include "lateral.h"
#include "centrocampista.h"
#include "mediapunta.h"
#include "extremo.h"
#include "entrenador.h"
#include "freeglut.h"
#include <cstdlib>
#include <ctime>

Mundo::Mundo() : balones{ Pelota(4, 4), Pelota(0, 4), Pelota(8, 4), Pelota(4, 0), Pelota(4, 8) },
                 equipoMadrid(18, nullptr),
                 equipoAtleti(18, nullptr)
{
    jugadorSeleccionado = nullptr;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            casillasValidas.at(i, j) = false;
}

Mundo::~Mundo()
{
    for (Jugador* pj : equipoMadrid)      delete pj;
    for (Jugador* pj : equipoAtleti)      delete pj;
    for (Jugador* pj : cementerioMadrid)  delete pj;
    for (Jugador* pj : cementerioAtleti)  delete pj;
}

void Mundo::inicializa()
{
    // --- REAL MADRID (Izquierda) ---

    // Línea frontal (Columna 1)
    equipoMadrid[0] = new Portero      (1, 0, 1); // Arquera abajo
    for (int i = 1; i < 8; i++)
        equipoMadrid[i] = new Delantero(1, (float)i, 1); // Caballeros
    equipoMadrid[8] = new Portero      (1, 8, 1); // Arquera arriba

    // Línea trasera (Columna 0)
    equipoMadrid[9]  = new Centrocampista(0, 0, 1); // Valkiria
    equipoMadrid[10] = new Central       (0, 1, 1); // Golem
    equipoMadrid[11] = new Lateral       (0, 2, 1); // Unicornio
    equipoMadrid[12] = new Mediapunta    (0, 3, 1); // Genio
    equipoMadrid[13] = new Entrenador    (0, 4, 1); // Mago
    equipoMadrid[14] = new Extremo       (0, 5, 1); // Fénix
    equipoMadrid[15] = new Lateral       (0, 6, 1); // Unicornio
    equipoMadrid[16] = new Central       (0, 7, 1); // Golem
    equipoMadrid[17] = new Centrocampista(0, 8, 1); // Valkiria

    // --- ATLETI (Derecha) ---

    // Línea frontal (Columna 7)
    equipoAtleti[0] = new Portero      (7, 0, 2); // Mantícora abajo
    for (int i = 1; i < 8; i++)
        equipoAtleti[i] = new Delantero(7, (float)i, 2); // Goblins
    equipoAtleti[8] = new Portero      (7, 8, 2); // Mantícora arriba

    // Línea trasera (Columna 8)
    equipoAtleti[9]  = new Centrocampista(8, 0, 2); // Banshee
    equipoAtleti[10] = new Central       (8, 1, 2); // Troll
    equipoAtleti[11] = new Lateral       (8, 2, 2); // Basilisco
    equipoAtleti[12] = new Mediapunta    (8, 3, 2); // Cambiaformas
    equipoAtleti[13] = new Entrenador    (8, 4, 2); // Hechicera
    equipoAtleti[14] = new Extremo       (8, 5, 2); // Dragón
    equipoAtleti[15] = new Lateral       (8, 6, 2); // Basilisco
    equipoAtleti[16] = new Central       (8, 7, 2); // Troll
    equipoAtleti[17] = new Centrocampista(8, 8, 2); // Banshee

    // Tirada de moneda: decide qué equipo saca primero
    srand((unsigned)time(nullptr));
    turnoEquipo = rand() % 2 + 1;
}

void Mundo::dibuja() const
{
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    miTablero.dibuja();

    // Casillas donde puede moverse la pieza seleccionada (amarillo semitransparente)
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 0.0f, 0.45f);
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (casillasValidas.at(i, j)) {
                glBegin(GL_QUADS);
                    glVertex2f((float)i,        (float)j);
                    glVertex2f((float)i + 1.0f, (float)j);
                    glVertex2f((float)i + 1.0f, (float)j + 1.0f);
                    glVertex2f((float)i,        (float)j + 1.0f);
                glEnd();
            }
    glDisable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
    for (int i = 0; i < 5; i++) balones[i].dibuja();

    if (jugadorSeleccionado != nullptr) {
        //NO TOCAR
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glDisable(GL_BLEND);

        glColor3f(1.0f, 1.0f, 0.0f); // Amarillo fosforescente puro

        float radio = 0.5f;
        glBegin(GL_QUADS);
        glVertex2f(jugadorSeleccionado->getPosX() - radio, jugadorSeleccionado->getPosY() - radio);
        glVertex2f(jugadorSeleccionado->getPosX() + radio, jugadorSeleccionado->getPosY() - radio);
        glVertex2f(jugadorSeleccionado->getPosX() + radio, jugadorSeleccionado->getPosY() + radio);
        glVertex2f(jugadorSeleccionado->getPosX() - radio, jugadorSeleccionado->getPosY() + radio);
        glEnd();

        glEnable(GL_TEXTURE_2D);
    }

    glDisable(GL_TEXTURE_2D);
    for (Jugador* pj : equipoMadrid) if (pj != nullptr) pj->dibuja();
    for (Jugador* pj : equipoAtleti) if (pj != nullptr) pj->dibuja();

    // Mensaje de turno
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glColor3f(1.0f, 1.0f, 1.0f);

    const char* msg;
    if (primerTurno)
        msg = (turnoEquipo == 1) ? "SACA REAL MADRID" : "SACA ATLETICO";
    else
        msg = (turnoEquipo == 1) ? "MUEVE REAL MADRID" : "MUEVE ATLETICO";

    glRasterPos2f(2.5f, -0.6f);
    for (const char* c = msg; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Piezas encarceladas (tinte rojo)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 0.0f, 0.0f, 0.35f);
    for (const Jugador* pj : equipoMadrid)
        if (pj != nullptr && pj->estaEncarcelado()) {
            float px = pj->getPosX() - 0.5f;
            float py = pj->getPosY() - 0.5f;
            glBegin(GL_QUADS);
                glVertex2f(px,        py);
                glVertex2f(px + 1.0f, py);
                glVertex2f(px + 1.0f, py + 1.0f);
                glVertex2f(px,        py + 1.0f);
            glEnd();
        }
    for (const Jugador* pj : equipoAtleti)
        if (pj != nullptr && pj->estaEncarcelado()) {
            float px = pj->getPosX() - 0.5f;
            float py = pj->getPosY() - 0.5f;
            glBegin(GL_QUADS);
                glVertex2f(px,        py);
                glVertex2f(px + 1.0f, py);
                glVertex2f(px + 1.0f, py + 1.0f);
                glVertex2f(px,        py + 1.0f);
            glEnd();
        }
    glDisable(GL_BLEND);

    // Panel de conjuros si el Entrenador esta seleccionado
    if (jugadorSeleccionado != nullptr && jugadorSeleccionado->esEntrenador()) {
        const Entrenador* ent = static_cast<const Entrenador*>(jugadorSeleccionado);

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);

        for (int i = 0; i < ent->numConjuros(); i++) {
            bool usado = ent->conjuroUsado(i);
            glColor3f(usado ? 0.35f : 1.0f,
                      usado ? 0.35f : 0.9f,
                      usado ? 0.35f : 0.2f);

            glRasterPos2f(-0.9f + i * 1.55f, -0.85f);
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, '[');
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, '1' + i);
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ']');
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ' ');
            for (const char* c = ent->nombreConjuro(i); *c; c++)
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
        }
    }
}

int Mundo::equipoEn(int x, int y) const
{
    for (const Jugador* pj : equipoMadrid)
        if (pj != nullptr && (int)pj->getPosX() == x && (int)pj->getPosY() == y)
            return 1;
    for (const Jugador* pj : equipoAtleti)
        if (pj != nullptr && (int)pj->getPosX() == x && (int)pj->getPosY() == y)
            return 2;
    return 0;
}

void Mundo::calcularCasillasValidas()
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            casillasValidas.at(i, j) = false;

    if (jugadorSeleccionado == nullptr) return;

    int gx       = (int)jugadorSeleccionado->getPosX();
    int gy       = (int)jugadorSeleccionado->getPosY();
    int rango    = jugadorSeleccionado->getRango();
    int miEquipo = jugadorSeleccionado->getEquipo();

    if (jugadorSeleccionado->esTeleport()) {
        // Puede ir a cualquier casilla que no esté ocupada por un aliado
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                if (equipoEn(i, j) != miEquipo)
                    casillasValidas.at(i, j) = true;
        casillasValidas.at(gx, gy) = false; // No puede quedarse donde está
    }
    else if (jugadorSeleccionado->esVolador()) {
        // 8 direcciones, pasa sobre piezas, para en aliado o enemigo
        int dirs[8][2] = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
        for (auto& d : dirs) {
            for (int r = 1; r <= rango; r++) {
                int nx = gx + d[0] * r;
                int ny = gy + d[1] * r;
                if (nx < 0 || nx >= 9 || ny < 0 || ny >= 9) break;
                int eq = equipoEn(nx, ny);
                if (eq == miEquipo) break;          // Bloqueado por aliado
                casillasValidas.at(nx, ny) = true;
                if (eq != 0) break;                 // Para en enemigo (puede aterrizar)
            }
        }
    }
    else {
        // Terrestre: 4 direcciones, bloqueado por cualquier pieza
        int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
        for (auto& d : dirs) {
            for (int r = 1; r <= rango; r++) {
                int nx = gx + d[0] * r;
                int ny = gy + d[1] * r;
                if (nx < 0 || nx >= 9 || ny < 0 || ny >= 9) break;
                int eq = equipoEn(nx, ny);
                if (eq == miEquipo) break;          // Bloqueado por aliado
                casillasValidas.at(nx, ny) = true;
                if (eq != 0) break;                 // Para en enemigo
            }
        }
    }
}

void Mundo::mueve() {}

void Mundo::tecla(unsigned char key)
{
    if (jugadorSeleccionado == nullptr) return;
    if (!jugadorSeleccionado->esEntrenador()) return;

    if (key >= '1' && key <= '7') {
        int idx = key - '1';
        Entrenador* ent = static_cast<Entrenador*>(jugadorSeleccionado);
        if (ent->lanzarConjuro(idx, *this)) {
            jugadorSeleccionado = nullptr;
            calcularCasillasValidas();
            primerTurno = false;
            turnoEquipo = (turnoEquipo == 1) ? 2 : 1;
            miTablero.avanzarCiclo();
            actualizarEncarcelados();
        }
    }
}

void Mundo::teclaEspecial(int key) {}

void Mundo::raton(int boton, int estado, float x, float y)
{
    if (boton != GLUT_LEFT_BUTTON || estado != GLUT_DOWN) return;

    int gx = (int)x;
    int gy = (int)y;

    // Click fuera del tablero → deseleccionar
    if (gx < 0 || gx >= 9 || gy < 0 || gy >= 9) {
        jugadorSeleccionado = nullptr;
        calcularCasillasValidas();
        return;
    }

    if (jugadorSeleccionado == nullptr) {
        // Primer click: buscar jugador del equipo en turno
        std::vector<Jugador*>& equipoEnTurno = (turnoEquipo == 1) ? equipoMadrid : equipoAtleti;
        for (Jugador* pj : equipoEnTurno) {
            if (pj != nullptr && !pj->estaEncarcelado()) {
                float difX = x - pj->getPosX();
                float difY = y - pj->getPosY();
                if (difX > -0.5f && difX < 0.5f && difY > -0.5f && difY < 0.5f)
                    jugadorSeleccionado = pj;
            }
        }
        calcularCasillasValidas();
    }
    else {
        // Segundo click: mover o iniciar combate si la casilla es valida
        if (casillasValidas.at(gx, gy)) {
            int equipoDestino = equipoEn(gx, gy);
            if (equipoDestino != 0 && equipoDestino != turnoEquipo) {
                // Hay un enemigo en la casilla destino: preparar combate
                std::vector<Jugador*>& enemigoArr = (equipoDestino == 1) ? equipoMadrid : equipoAtleti;
                for (Jugador* pj : enemigoArr) {
                    if (pj != nullptr &&
                        (int)pj->getPosX() == gx &&
                        (int)pj->getPosY() == gy) {
                        pendientePj1 = jugadorSeleccionado;
                        pendientePj2 = pj;
                        destCombateX = gx;
                        destCombateY = gy;
                        break;
                    }
                }
                // No se mueve todavia: el coordinador activara la arena
            } else {
                // Casilla libre: mover y cambiar turno
                jugadorSeleccionado->moverA((float)gx, (float)gy);
                primerTurno = false;
                turnoEquipo = (turnoEquipo == 1) ? 2 : 1;
                miTablero.avanzarCiclo();
                actualizarEncarcelados();
            }
        }
        jugadorSeleccionado = nullptr;
        calcularCasillasValidas();
    }
}

void Mundo::eliminarPieza(Jugador* pj)
{
    for (Jugador*& slot : equipoMadrid)
        if (slot == pj) { cementerioMadrid.push_back(slot); slot = nullptr; return; }
    for (Jugador*& slot : equipoAtleti)
        if (slot == pj) { cementerioAtleti.push_back(slot); slot = nullptr; return; }
}

void Mundo::resolverCombate(int equipoGanador)
{
    if (pendientePj1 == nullptr || pendientePj2 == nullptr) return;

    if (equipoGanador == pendientePj1->getEquipo()) {
        // Atacante gana: elimina al defensor y ocupa su casilla
        eliminarPieza(pendientePj2);
        pendientePj1->moverA((float)destCombateX, (float)destCombateY);
    } else {
        // Defensor gana: elimina al atacante
        eliminarPieza(pendientePj1);
    }

    primerTurno = false;
    turnoEquipo = (turnoEquipo == 1) ? 2 : 1;
    miTablero.avanzarCiclo();
    actualizarEncarcelados();
    limpiarCombatePendiente();
}

void Mundo::limpiarCombatePendiente()
{
    pendientePj1 = nullptr;
    pendientePj2 = nullptr;
    destCombateX = -1;
    destCombateY = -1;
}

void Mundo::invertirCiclo()
{
    miTablero.invierteDireccionCiclo();
}

void Mundo::curarEquipoCompleto(int equipo)
{
    std::vector<Jugador*>& equip = (equipo == 1) ? equipoMadrid : equipoAtleti;
    for (Jugador* pj : equip)
        if (pj != nullptr) pj->curarCompleto();
}

bool Mundo::teleportarAleatoriamente(int equipo)
{
    std::vector<Jugador*>& miEquip = (equipo == 1) ? equipoMadrid : equipoAtleti;
    Jugador* objetivo = nullptr;
    for (Jugador* pj : miEquip)
        if (pj != nullptr && !pj->esEntrenador()) { objetivo = pj; break; }
    if (objetivo == nullptr) return false;

    for (int intento = 0; intento < 40; intento++) {
        int tx = rand() % 9;
        int ty = rand() % 9;
        if (equipoEn(tx, ty) == 0) {
            objetivo->moverA((float)tx, (float)ty);
            return true;
        }
    }
    return false;
}

bool Mundo::intercambiarPiezas(int equipo)
{
    std::vector<Jugador*>& miEquip = (equipo == 1) ? equipoMadrid : equipoAtleti;
    Jugador* pA = nullptr;
    Jugador* pB = nullptr;
    for (Jugador* pj : miEquip) {
        if (pj == nullptr) continue;
        if (pA == nullptr)       pA = pj;
        else if (pB == nullptr) { pB = pj; break; }
    }
    if (pA == nullptr || pB == nullptr) return false;

    float ax = pA->getPosX() - 0.5f,  ay = pA->getPosY() - 0.5f;
    float bx = pB->getPosX() - 0.5f,  by = pB->getPosY() - 0.5f;
    pA->moverA(bx, by);
    pB->moverA(ax, ay);
    return true;
}

void Mundo::actualizarEncarcelados()
{
    for (Jugador* pj : equipoMadrid)
        if (pj != nullptr) pj->descuentaCicloEncarcelado();
    for (Jugador* pj : equipoAtleti)
        if (pj != nullptr) pj->descuentaCicloEncarcelado();
}

bool Mundo::encarcelarEnemigo(int equipo)
{
    int rival = (equipo == 1) ? 2 : 1;
    std::vector<Jugador*>& rivalArr = (rival == 1) ? equipoMadrid : equipoAtleti;
    for (Jugador* pj : rivalArr)
        if (pj != nullptr && !pj->esEntrenador() && !pj->estaEncarcelado()) {
            pj->encarcelar(3);
            return true;
        }
    return false;
}

bool Mundo::invocarElemental(int equipo)
{
    std::vector<Jugador*>& miEquip = (equipo == 1) ? equipoMadrid : equipoAtleti;
    Jugador* entrenador = nullptr;
    for (Jugador* pj : miEquip)
        if (pj != nullptr && pj->esEntrenador()) { entrenador = pj; break; }
    if (entrenador == nullptr) return false;

    int ex = (int)(entrenador->getPosX() - 0.5f);
    int ey = (int)(entrenador->getPosY() - 0.5f);
    int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    for (auto& d : dirs) {
        int nx = ex + d[0];
        int ny = ey + d[1];
        if (nx >= 0 && nx < 9 && ny >= 0 && ny < 9 && equipoEn(nx, ny) == 0) {
            Jugador* elemental = new Delantero((float)nx, (float)ny, equipo);
            for (Jugador*& slot : miEquip)
                if (slot == nullptr) { slot = elemental; return true; }
            miEquip.push_back(elemental);
            return true;
        }
    }
    return false;
}

bool Mundo::revivirPieza(int equipo)
{
    std::vector<Jugador*>& cementerio = (equipo == 1) ? cementerioMadrid : cementerioAtleti;
    if (cementerio.empty()) return false;

    Jugador* revivida = cementerio.back();
    cementerio.pop_back();
    revivida->curarCompleto();

    std::vector<Jugador*>& miEquip = (equipo == 1) ? equipoMadrid : equipoAtleti;

    Jugador* entrenador = nullptr;
    for (Jugador* pj : miEquip)
        if (pj != nullptr && pj->esEntrenador()) { entrenador = pj; break; }

    if (entrenador != nullptr) {
        int ex = (int)(entrenador->getPosX() - 0.5f);
        int ey = (int)(entrenador->getPosY() - 0.5f);
        int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
        for (auto& d : dirs) {
            int nx = ex + d[0];
            int ny = ey + d[1];
            if (nx >= 0 && nx < 9 && ny >= 0 && ny < 9 && equipoEn(nx, ny) == 0) {
                revivida->moverA((float)nx, (float)ny);
                for (Jugador*& slot : miEquip)
                    if (slot == nullptr) { slot = revivida; return true; }
                miEquip.push_back(revivida);
                return true;
            }
        }
    }

    // Si no hay casilla libre junto al entrenador, busca en el tablero
    for (int intento = 0; intento < 40; intento++) {
        int tx = rand() % 9;
        int ty = rand() % 9;
        if (equipoEn(tx, ty) == 0) {
            revivida->moverA((float)tx, (float)ty);
            for (Jugador*& slot : miEquip)
                if (slot == nullptr) { slot = revivida; return true; }
            miEquip.push_back(revivida);
            return true;
        }
    }

    cementerio.push_back(revivida);
    return false;
}

int Mundo::comprobarVictoria() const
{
    // Condicion 1: controlar los 5 puntos de poder
    int puntosX[5] = { 4, 0, 8, 4, 4 };
    int puntosY[5] = { 4, 4, 4, 0, 8 };
    int controlMadrid = 0;
    int controlAtleti = 0;
    for (int i = 0; i < 5; i++) {
        int eq = equipoEn(puntosX[i], puntosY[i]);
        if (eq == 1) controlMadrid++;
        if (eq == 2) controlAtleti++;
    }
    if (controlMadrid == 5) return 1;
    if (controlAtleti == 5) return 2;

    // Condicion 2: eliminar todas las piezas del rival
    int vivasMadrid = 0;
    int vivasAtleti = 0;
    for (const Jugador* pj : equipoMadrid) if (pj != nullptr) vivasMadrid++;
    for (const Jugador* pj : equipoAtleti) if (pj != nullptr) vivasAtleti++;
    if (vivasMadrid == 0) return 2;
    if (vivasAtleti == 0) return 1;

    // Condicion 3: dejar al rival con solo una pieza encarcelada
    if (vivasAtleti == 1)
        for (const Jugador* pj : equipoAtleti)
            if (pj != nullptr && pj->estaEncarcelado()) return 1;
    if (vivasMadrid == 1)
        for (const Jugador* pj : equipoMadrid)
            if (pj != nullptr && pj->estaEncarcelado()) return 2;

    return 0;
}
