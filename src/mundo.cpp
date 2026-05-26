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

Mundo::Mundo() : balones{ Pelota(4, 4), Pelota(0, 4), Pelota(8, 4), Pelota(4, 0), Pelota(4, 8) }
{
    jugadorSeleccionado = nullptr;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            casillasValidas[i][j] = false;
    for (int i = 0; i < 18; i++) {
        equipoMadrid[i] = nullptr;
        equipoAtleti[i] = nullptr;
    }
}

Mundo::~Mundo()
{
    for (int i = 0; i < 18; i++) {
        if (equipoMadrid[i] != nullptr) delete equipoMadrid[i];
        if (equipoAtleti[i] != nullptr) delete equipoAtleti[i];
    }
}

void Mundo::inicializa()
{
    // --- REAL MADRID (Izquierda) ---

    // Línea frontal (Columna 1)
    equipoMadrid[0] = new Portero      (1, 0, 1); // Arquera abajo
    for (int i = 1; i < 8; i++)
        equipoMadrid[i] = new Delantero(1, i, 1); // Caballeros
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
        equipoAtleti[i] = new Delantero(7, i, 2); // Goblins
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
            if (casillasValidas[i][j]) {
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
        glVertex2f(jugadorSeleccionado->pos.x - radio, jugadorSeleccionado->pos.y - radio);
        glVertex2f(jugadorSeleccionado->pos.x + radio, jugadorSeleccionado->pos.y - radio);
        glVertex2f(jugadorSeleccionado->pos.x + radio, jugadorSeleccionado->pos.y + radio);
        glVertex2f(jugadorSeleccionado->pos.x - radio, jugadorSeleccionado->pos.y + radio);
        glEnd();

        glEnable(GL_TEXTURE_2D);
    }

    glDisable(GL_TEXTURE_2D);
    for (int i = 0; i < 18; i++) {
        if (equipoMadrid[i] != nullptr) equipoMadrid[i]->dibuja();
        if (equipoAtleti[i] != nullptr) equipoAtleti[i]->dibuja();
    }

    // Mensaje de turno
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glColor3f(1.0f, 1.0f, 1.0f);

    const char* msg;
    if (primerTurno)
        msg = (turnoEquipo == 1) ? "SACA EQUIPO BLANCO" : "SACA EQUIPO ROJO";
    else
        msg = (turnoEquipo == 1) ? "MUEVE EQUIPO BLANCO" : "MUEVE EQUIPO ROJO";

    glRasterPos2f(2.5f, -0.6f);
    for (const char* c = msg; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

int Mundo::equipoEn(int x, int y) const
{
    for (int i = 0; i < 18; i++) {
        if (equipoMadrid[i] != nullptr)
            if ((int)equipoMadrid[i]->pos.x == x && (int)equipoMadrid[i]->pos.y == y)
                return 1;
        if (equipoAtleti[i] != nullptr)
            if ((int)equipoAtleti[i]->pos.x == x && (int)equipoAtleti[i]->pos.y == y)
                return 2;
    }
    return 0;
}

Jugador* Mundo::buscarJugadorEn(int x, int y) const
{
    for (int i = 0; i < 18; i++) {
        if (equipoMadrid[i] != nullptr &&
            (int)equipoMadrid[i]->pos.x == x && (int)equipoMadrid[i]->pos.y == y)
            return equipoMadrid[i];
        if (equipoAtleti[i] != nullptr &&
            (int)equipoAtleti[i]->pos.x == x && (int)equipoAtleti[i]->pos.y == y)
            return equipoAtleti[i];
    }
    return nullptr;
}

void Mundo::consumirCombate()
{
    combatePendiente = false;
    atacante = nullptr;
    defensor = nullptr;
}

void Mundo::eliminarPerdedor(Jugador* perdedor)
{
    if (perdedor == nullptr) return;
    for (int i = 0; i < 18; i++) {
        if (equipoMadrid[i] == perdedor) {
            delete equipoMadrid[i];
            equipoMadrid[i] = nullptr;
            return;
        }
        if (equipoAtleti[i] == perdedor) {
            delete equipoAtleti[i];
            equipoAtleti[i] = nullptr;
            return;
        }
    }
}

void Mundo::pasarTurno()
{
    turnoEquipo = (turnoEquipo == 1) ? 2 : 1;
}

void Mundo::calcularCasillasValidas()
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            casillasValidas[i][j] = false;

    if (jugadorSeleccionado == nullptr) return;

    int gx       = (int)jugadorSeleccionado->pos.x;
    int gy       = (int)jugadorSeleccionado->pos.y;
    int rango    = jugadorSeleccionado->getRango();
    int miEquipo = jugadorSeleccionado->getEquipo();

    if (jugadorSeleccionado->esTeleport()) {
        // Puede ir a cualquier casilla que no esté ocupada por un aliado
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                if (equipoEn(i, j) != miEquipo)
                    casillasValidas[i][j] = true;
        casillasValidas[gx][gy] = false; // No puede quedarse donde está
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
                casillasValidas[nx][ny] = true;
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
                casillasValidas[nx][ny] = true;
                if (eq != 0) break;                 // Para en enemigo
            }
        }
    }
}

void Mundo::mueve() {}

void Mundo::tecla(unsigned char key) {}

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
        if (turnoEquipo == 1) {
            for (int i = 0; i < 18; i++) {
                if (equipoMadrid[i] != nullptr) {
                    float difX = x - equipoMadrid[i]->pos.x;
                    float difY = y - equipoMadrid[i]->pos.y;
                    if (difX > -0.5f && difX < 0.5f && difY > -0.5f && difY < 0.5f)
                        jugadorSeleccionado = equipoMadrid[i];
                }
            }
        }
        else {
            for (int i = 0; i < 18; i++) {
                if (equipoAtleti[i] != nullptr) {
                    float difX = x - equipoAtleti[i]->pos.x;
                    float difY = y - equipoAtleti[i]->pos.y;
                    if (difX > -0.5f && difX < 0.5f && difY > -0.5f && difY < 0.5f)
                        jugadorSeleccionado = equipoAtleti[i];
                }
            }
        }
        calcularCasillasValidas();
    }
    else {
        // Segundo click: mover si la casilla es válida
        if (casillasValidas[gx][gy]) {
            // Comprobar si hay enemigo en el destino ANTES de mover
            int eqDestino = equipoEn(gx, gy);
            Jugador* def = nullptr;
            if (eqDestino != 0 && eqDestino != jugadorSeleccionado->getEquipo())
                def = buscarJugadorEn(gx, gy);

            jugadorSeleccionado->moverA((float)gx, (float)gy);
            primerTurno = false;

            if (def != nullptr) {
                // Hay combate: no cambiar turno hasta que la arena devuelva resultado
                combatePendiente = true;
                atacante = jugadorSeleccionado;
                defensor = def;
            } else {
                turnoEquipo = (turnoEquipo == 1) ? 2 : 1;
            }
        }
        jugadorSeleccionado = nullptr;
        calcularCasillasValidas();
    }
}
