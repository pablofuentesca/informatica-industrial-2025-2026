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
                 madrid(1, "REAL MADRID"),
                 atleti(2, "ATLETICO")
{
    jugadorSeleccionado = nullptr;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            casillasValidas.at(i, j) = false;
}

Mundo::~Mundo()
{
    // cada Equipo libera sus propias piezas y cementerio en su destructor
}

void Mundo::inicializa()
{
    // --- REAL MADRID (Izquierda) ---

    // Línea frontal (Columna 1)
    madrid.coloca(0, new Portero      (1, 0, 1)); // Arquera abajo
    for (int i = 1; i < 8; i++)
        madrid.coloca(i, new Delantero(1, (float)i, 1)); // Caballeros
    madrid.coloca(8, new Portero      (1, 8, 1)); // Arquera arriba

    // Línea trasera (Columna 0)
    madrid.coloca(9,  new Centrocampista(0, 0, 1)); // Valkiria
    madrid.coloca(10, new Central       (0, 1, 1)); // Golem
    madrid.coloca(11, new Lateral       (0, 2, 1)); // Unicornio
    madrid.coloca(12, new Mediapunta    (0, 3, 1)); // Genio
    madrid.coloca(13, new Entrenador    (0, 4, 1)); // Mago
    madrid.coloca(14, new Extremo       (0, 5, 1)); // Fénix
    madrid.coloca(15, new Lateral       (0, 6, 1)); // Unicornio
    madrid.coloca(16, new Central       (0, 7, 1)); // Golem
    madrid.coloca(17, new Centrocampista(0, 8, 1)); // Valkiria

    // --- ATLETI (Derecha) ---

    // Línea frontal (Columna 7)
    atleti.coloca(0, new Portero      (7, 0, 2)); // Mantícora abajo
    for (int i = 1; i < 8; i++)
        atleti.coloca(i, new Delantero(7, (float)i, 2)); // Goblins
    atleti.coloca(8, new Portero      (7, 8, 2)); // Mantícora arriba

    // Línea trasera (Columna 8)
    atleti.coloca(9,  new Centrocampista(8, 0, 2)); // Banshee
    atleti.coloca(10, new Central       (8, 1, 2)); // Troll
    atleti.coloca(11, new Lateral       (8, 2, 2)); // Basilisco
    atleti.coloca(12, new Mediapunta    (8, 3, 2)); // Cambiaformas
    atleti.coloca(13, new Entrenador    (8, 4, 2)); // Hechicera
    atleti.coloca(14, new Extremo       (8, 5, 2)); // Dragón
    atleti.coloca(15, new Lateral       (8, 6, 2)); // Basilisco
    atleti.coloca(16, new Central       (8, 7, 2)); // Troll
    atleti.coloca(17, new Centrocampista(8, 8, 2)); // Banshee

    // Tirada de moneda: decide qué equipo saca primero
    turno.sortea();
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
    for (Jugador* pj : madrid.getPiezas()) if (pj != nullptr) pj->dibuja();
    for (Jugador* pj : atleti.getPiezas()) if (pj != nullptr) pj->dibuja();

    // Mensaje de turno
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glColor3f(1.0f, 1.0f, 1.0f);

    const char* msg;
    if (turno.esPrimerTurno())
        msg = (turno.equipoEnJuego() == 1) ? "SACA REAL MADRID" : "SACA ATLETICO";
    else
        msg = (turno.equipoEnJuego() == 1) ? "MUEVE REAL MADRID" : "MUEVE ATLETICO";

    glRasterPos2f(2.5f, -0.6f);
    for (const char* c = msg; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Piezas encarceladas (tinte rojo)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 0.0f, 0.0f, 0.35f);
    for (const Jugador* pj : madrid.getPiezas())
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
    for (const Jugador* pj : atleti.getPiezas())
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
    if (madrid.piezaEn(x, y) != nullptr) return 1;
    if (atleti.piezaEn(x, y) != nullptr) return 2;
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
            cierraTurno();
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
        const std::vector<Jugador*>& equipoEnTurno = equipoPorId(turno.equipoEnJuego()).getPiezas();
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
            if (equipoDestino != 0 && equipoDestino != turno.equipoEnJuego()) {
                // Hay un enemigo en la casilla destino: preparar combate
                Jugador* defensor = equipoPorId(equipoDestino).piezaEn(gx, gy);
                if (defensor != nullptr) {
                    pendientePj1 = jugadorSeleccionado;
                    pendientePj2 = defensor;
                    destCombateX = gx;
                    destCombateY = gy;
                }
                // No se mueve todavia: el coordinador activara la arena
            } else {
                // Casilla libre: mover y cambiar turno
                jugadorSeleccionado->moverA((float)gx, (float)gy);
                cierraTurno();
            }
        }
        jugadorSeleccionado = nullptr;
        calcularCasillasValidas();
    }
}

void Mundo::eliminarPieza(Jugador* pj)
{
    if (pj != nullptr) equipoPorId(pj->getEquipo()).elimina(pj);
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

    cierraTurno();
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

// cierra la jugada actual: pasa el turno, avanza el ciclo de luz del tablero
// y descuenta un ciclo de carcel a las piezas encarceladas de ambos equipos
void Mundo::cierraTurno()
{
    turno.cambia();
    miTablero.avanzarCiclo();
    madrid.descuentaEncarcelados();
    atleti.descuentaEncarcelados();
}

void Mundo::curarEquipoCompleto(int equipo)
{
    equipoPorId(equipo).curaTodos();
}

bool Mundo::teleportarAleatoriamente(int equipo)
{
    const std::vector<Jugador*>& miEquip = equipoPorId(equipo).getPiezas();
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
    const std::vector<Jugador*>& miEquip = equipoPorId(equipo).getPiezas();
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

bool Mundo::encarcelarEnemigo(int equipo)
{
    int rival = (equipo == 1) ? 2 : 1;
    for (Jugador* pj : equipoPorId(rival).getPiezas())
        if (pj != nullptr && !pj->esEntrenador() && !pj->estaEncarcelado()) {
            pj->encarcelar(3);
            return true;
        }
    return false;
}

bool Mundo::invocarElemental(int equipo)
{
    Equipo& miEquip = equipoPorId(equipo);
    Jugador* entrenador = miEquip.buscaEntrenador();
    if (entrenador == nullptr) return false;

    int ex = (int)(entrenador->getPosX() - 0.5f);
    int ey = (int)(entrenador->getPosY() - 0.5f);
    int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    for (auto& d : dirs) {
        int nx = ex + d[0];
        int ny = ey + d[1];
        if (nx >= 0 && nx < 9 && ny >= 0 && ny < 9 && equipoEn(nx, ny) == 0) {
            miEquip.anade(new Delantero((float)nx, (float)ny, equipo));
            return true;
        }
    }
    return false;
}

bool Mundo::revivirPieza(int equipo)
{
    Equipo& miEquip = equipoPorId(equipo);
    Jugador* revivida = miEquip.ultimaEnCementerio();
    if (revivida == nullptr) return false;

    Jugador* entrenador = miEquip.buscaEntrenador();
    if (entrenador != nullptr) {
        int ex = (int)(entrenador->getPosX() - 0.5f);
        int ey = (int)(entrenador->getPosY() - 0.5f);
        int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
        for (auto& d : dirs) {
            int nx = ex + d[0];
            int ny = ey + d[1];
            if (nx >= 0 && nx < 9 && ny >= 0 && ny < 9 && equipoEn(nx, ny) == 0) {
                miEquip.sacaDeCementerio();
                revivida->curarCompleto();
                revivida->moverA((float)nx, (float)ny);
                miEquip.anade(revivida);
                return true;
            }
        }
    }

    // Si no hay casilla libre junto al entrenador, busca en el tablero
    for (int intento = 0; intento < 40; intento++) {
        int tx = rand() % 9;
        int ty = rand() % 9;
        if (equipoEn(tx, ty) == 0) {
            miEquip.sacaDeCementerio();
            revivida->curarCompleto();
            revivida->moverA((float)tx, (float)ty);
            miEquip.anade(revivida);
            return true;
        }
    }

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
    for (const Jugador* pj : madrid.getPiezas()) if (pj != nullptr) vivasMadrid++;
    for (const Jugador* pj : atleti.getPiezas()) if (pj != nullptr) vivasAtleti++;
    if (vivasMadrid == 0) return 2;
    if (vivasAtleti == 0) return 1;

    // Condicion 3: dejar al rival con solo una pieza encarcelada
    if (vivasAtleti == 1)
        for (const Jugador* pj : atleti.getPiezas())
            if (pj != nullptr && pj->estaEncarcelado()) return 1;
    if (vivasMadrid == 1)
        for (const Jugador* pj : madrid.getPiezas())
            if (pj != nullptr && pj->estaEncarcelado()) return 2;

    return 0;
}
