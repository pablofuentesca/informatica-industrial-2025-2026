#include "mundo.h"
#include "vistaJugador.h"
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

static void textoPanel(float x, float y, const char* s, void* fuente,float r, float g, float b)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (const char* c = s; *c; c++) glutBitmapCharacter(fuente, *c);
}

Mundo::Mundo() : balones{ Pelota(4, 4), Pelota(0, 4), Pelota(8, 4), Pelota(4, 0), Pelota(4, 8) },madrid(1, "REAL MADRID"), atleti(2, "ATLETICO")
{
    jugadorSeleccionado = nullptr;
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++)
        {
            casillasValidas.at(i, j) = false;
        }
     }
}

Mundo::~Mundo()
{
    // cada Equipo libera sus propias piezas y cementerio en su destructor
    for (auto& par : vistas) delete par.second;
}

void Mundo::inicializa()
{
    // REAL MADRID (Izquierda)

    // Línea frontal (Columna 1)
    madrid.coloca(0, new Arquero(1, 0));
    for (int i = 1; i < 8; i++)
        madrid.coloca(i, new Caballero(1, (float)i));
    madrid.coloca(8, new Arquero (1, 8));

    // Línea trasera (Columna 0)
    madrid.coloca(9, new Valkiria (0, 0));
    madrid.coloca(10, new Golem (0, 1));
    madrid.coloca(11, new Unicornio(0, 2));
    madrid.coloca(12, new Genio (0, 3));
    madrid.coloca(13, new Mago (0, 4));
    madrid.coloca(14, new Fenix (0, 5));
    madrid.coloca(15, new Unicornio(0, 6));
    madrid.coloca(16, new Golem (0, 7));
    madrid.coloca(17, new Valkiria (0, 8));

    // ATLETI (Derecha)

    // Línea frontal (Columna 7)
    atleti.coloca(0, new Manticora  (7, 0));
    for (int i = 1; i < 8; i++)
        atleti.coloca(i, new Goblin (7, (float)i));
    atleti.coloca(8, new Manticora  (7, 8));

    // Línea trasera (Columna 8)
    atleti.coloca(9, new Banshee (8, 0));
    atleti.coloca(10, new Trol (8, 1));
    atleti.coloca(11, new Basilisco(8, 2));
    atleti.coloca(12, new Cambiaformas(8, 3));
    atleti.coloca(13, new Hechicera (8, 4));
    atleti.coloca(14, new Dragon (8, 5));
    atleti.coloca(15, new Basilisco(8, 6));
    atleti.coloca(16, new Trol(8, 7));
    atleti.coloca(17, new Banshee (8, 8));

    // Tirada de moneda: decide qué equipo saca primero
    turno.sortea();

    // Crear una VistaJugador por cada pieza: el sprite vive aqui, no en Jugador
    static const float RADIO = 0.4f;
    for (Jugador* pj : madrid.getPiezas())
    {
        if (pj != nullptr)
            vistas[pj] = new VistaJugador(pj->getRutaTextura(), pj->getPosX(), pj->getPosY(), RADIO);
    }
    for (Jugador* pj : atleti.getPiezas())
    {
     if (pj != nullptr)
            vistas[pj] = new VistaJugador(pj->getRutaTextura(), pj->getPosX(), pj->getPosY(), RADIO);
     }
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
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (casillasValidas.at(i, j)) {
                glBegin(GL_QUADS);
                glVertex2f((float)i,(float)j);
                glVertex2f((float)i + 1.0f,(float)j);
                glVertex2f((float)i + 1.0f,(float)j + 1.0f);
                glVertex2f((float)i,(float)j + 1.0f);
                glEnd();
            }
        }
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
    for (Jugador* pj : madrid.getPiezas())
        if (pj != nullptr) {
            auto it = vistas.find(pj);
            if (it != vistas.end()) it->second->dibuja(pj->getPosX(), pj->getPosY());
        }
    for (Jugador* pj : atleti.getPiezas())
        if (pj != nullptr) {
            auto it = vistas.find(pj);
            if (it != vistas.end()) it->second->dibuja(pj->getPosX(), pj->getPosY());
        }

    // Mensaje de turno
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glColor3f(1.0f, 1.0f, 1.0f);

    const char* msg;
    if      (faseConjuro == FaseConjuro::TELEPORT_PIEZA) msg = "TELEPORT: elige la pieza a mover";
    else if (faseConjuro == FaseConjuro::TELEPORT_DESTINO) msg = "TELEPORT: elige el destino";
    else if (faseConjuro == FaseConjuro::EXCHANGE_PRIMERA) msg = "INTERCAMBIAR: elige la primera pieza";
    else if (faseConjuro == FaseConjuro::EXCHANGE_SEGUNDA) msg = "INTERCAMBIAR: elige la segunda pieza";
    else if (turno.esPrimerTurno())
        msg = (turno.equipoEnJuego() == 1) ? "SACA REAL MADRID" : "SACA ATLETICO";
    else
        msg = (turno.equipoEnJuego() == 1) ? "MUEVE REAL MADRID" : "MUEVE ATLETICO";

    // Piezas encarceladas (tinte rojo)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 0.0f, 0.0f, 0.35f);
    for (const Jugador* pj : madrid.getPiezas())
        if (pj != nullptr && pj->estaEncarcelado()) {
            float px = pj->getPosX() - 0.5f;
            float py = pj->getPosY() - 0.5f;
            glBegin(GL_QUADS);
            glVertex2f(px,py);
            glVertex2f(px + 1.0f,py);
            glVertex2f(px + 1.0f,py + 1.0f);
            glVertex2f(px,py + 1.0f);
            glEnd();
        }
    for (const Jugador* pj : atleti.getPiezas())
        if (pj != nullptr && pj->estaEncarcelado()) {
            float px = pj->getPosX() - 0.5f;
            float py = pj->getPosY() - 0.5f;
            glBegin(GL_QUADS);
            glVertex2f(px,py);
            glVertex2f(px + 1.0f,py);
            glVertex2f(px + 1.0f,py + 1.0f);
            glVertex2f(px, py + 1.0f);
            glEnd();
        }
    glDisable(GL_BLEND);

    {
        glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
        gluOrtho2D(0, 800, 0, 600);
        glMatrixMode(GL_MODELVIEW);  glPushMatrix(); glLoadIdentity();
        glDisable(GL_TEXTURE_2D);

        bool hayEntrenador = (jugadorSeleccionado != nullptr && jugadorSeleccionado->esEntrenador());
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.03f, 0.05f, 0.09f, 0.92f);
        glBegin(GL_QUADS);
        glVertex2f(0, 560); glVertex2f(800, 560); glVertex2f(800, 598); glVertex2f(0, 598);
        glEnd();
        glColor3f(0.70f, 0.55f, 0.10f);
        glBegin(GL_LINES); glVertex2f(0, 560); glVertex2f(800, 560); glEnd();
        glDisable(GL_BLEND);

        int ancho = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)msg);
        textoPanel(400 - ancho / 2.0f, 572, msg, GLUT_BITMAP_HELVETICA_18, 1.0f, 1.0f, 1.0f);
        if (hayEntrenador)
            textoPanel(690, 573, "[8] Ayuda", GLUT_BITMAP_HELVETICA_12, 0.55f, 0.80f, 0.95f);

        if (hayEntrenador) {
            const Entrenador* ent = static_cast<const Entrenador*>(jugadorSeleccionado);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(0.03f, 0.05f, 0.09f, 0.92f);
            glBegin(GL_QUADS);
            glVertex2f(0, 4); glVertex2f(800, 4); glVertex2f(800, 40); glVertex2f(0, 40);
            glEnd();
            glColor3f(0.70f, 0.55f, 0.10f);
            glBegin(GL_LINES); glVertex2f(0, 40); glVertex2f(800, 40); glEnd();
            glDisable(GL_BLEND);

            for (int i = 0; i < ent->numConjuros(); i++) {
                bool usado = ent->conjuroUsado(i);
                float r = usado ? 0.40f : 0.95f;
                float g = usado ? 0.40f : 0.80f;
                float b = usado ? 0.40f : 0.30f;

                char etiqueta[40];
                etiqueta[0] = '['; etiqueta[1] = (char)('1' + i);
                etiqueta[2] = ']'; etiqueta[3] = ' ';
                int k = 4;
                for (const char* c = ent->nombreConjuro(i); *c && k < 39; c++)
                etiqueta[k++] = *c;
                etiqueta[k] = '\0';

                textoPanel(12 + i * 112.0f, 14, etiqueta, GLUT_BITMAP_HELVETICA_12, r, g, b);
            }
        }

        glMatrixMode(GL_PROJECTION); glPopMatrix();
        glMatrixMode(GL_MODELVIEW); glPopMatrix();
    }

    if (mostrarHechizos) {
        glMatrixMode(GL_PROJECTION);glPushMatrix(); glLoadIdentity();
        gluOrtho2D(0, 800, 0, 600);
        glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
        glBegin(GL_QUADS);
        glVertex2f(0, 0); glVertex2f(800, 0); glVertex2f(800, 600); glVertex2f(0, 600);
        glEnd();

        const float x0 = 90, x1 = 710, y0 = 95, y1 = 505;

        glColor4f(0.04f, 0.06f, 0.10f, 0.97f);
        glBegin(GL_QUADS);
        glVertex2f(x0, y0); glVertex2f(x1, y0); glVertex2f(x1, y1); glVertex2f(x0, y1);
        glEnd();

        glColor3f(0.90f, 0.90f, 0.95f);
        glBegin(GL_QUADS);
        glVertex2f(x0, y1 - 8); glVertex2f(400, y1 - 8); glVertex2f(400, y1); glVertex2f(x0, y1);
        glEnd();
        glColor3f(0.70f, 0.10f, 0.15f);
        glBegin(GL_QUADS);
        glVertex2f(400, y1 - 8); glVertex2f(x1, y1 - 8); glVertex2f(x1, y1); glVertex2f(400, y1);
        glEnd();

        glColor3f(0.82f, 0.66f, 0.12f);
        glLineWidth(2.5f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x0, y0); glVertex2f(x1, y0); glVertex2f(x1, y1); glVertex2f(x0, y1);
        glEnd();
        glColor3f(0.45f, 0.36f, 0.06f);
        glLineWidth(1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x0 + 6, y0 + 6); glVertex2f(x1 - 6, y0 + 6);
        glVertex2f(x1 - 6, y1 - 14); glVertex2f(x0 + 6, y1 - 14);
        glEnd();
        glDisable(GL_BLEND);

        textoPanel(272, 470, "HECHIZOS DEL ENTRENADOR", GLUT_BITMAP_HELVETICA_18, 0.95f, 0.82f, 0.25f);
        glColor3f(0.50f, 0.40f, 0.08f);
        glBegin(GL_LINES); glVertex2f(265, 462); glVertex2f(540, 462); glEnd();

        struct { const char* etiqueta; const char* desc; } h[7] = {
            { "[1] Teleport","mueve una pieza tuya a cualquier casilla libre que elijas." },
            { "[2] Curar","devuelve toda la vida a tu pieza mas debilitada." },
            { "[3] Ciclo","invierte la luz del tablero (las casillas cambian de color)." },
            { "[4] Intercambiar","cambia de posicion dos piezas tuyas que elijas." },
            { "[5] Elemental","invoca una pieza aliada temporal junto al entrenador." },
            { "[6] Revivir","recupera tu ultima pieza eliminada, junto al entrenador." },
            { "[7] Encarcelar","inmoviliza 3 turnos a una pieza enemiga rival." }
        };

        float y = 425;
        for (int i = 0; i < 7; i++) {
            textoPanel(115, y, h[i].etiqueta,GLUT_BITMAP_HELVETICA_12, 0.95f, 0.80f, 0.30f);
            textoPanel(250, y, h[i].desc, GLUT_BITMAP_HELVETICA_12, 0.85f, 0.87f, 0.92f);
            y -= 45;
        }

        textoPanel(205, 122, "Cada hechizo solo se usa UNA vez por partida.", GLUT_BITMAP_HELVETICA_12, 0.60f, 0.60f, 0.62f);
        textoPanel(312, 104, "Pulsa 8 para cerrar",GLUT_BITMAP_HELVETICA_12, 0.55f, 0.55f, 0.57f);

        glMatrixMode(GL_PROJECTION); glPopMatrix();
        glMatrixMode(GL_MODELVIEW); glPopMatrix();
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
    {
        for (int j = 0; j < 9; j++)
        {
            casillasValidas.at(i, j) = false;
        }
    }
    if (jugadorSeleccionado == nullptr) return;

    int gx = (int)jugadorSeleccionado->getPosX();
    int gy = (int)jugadorSeleccionado->getPosY();
    int miEquipo = jugadorSeleccionado->getEquipo();

	for (int i = 0; i < 9; i++) 
    {  //Utilizamos un bucle doble para recorrer todas las casillas del tablero y comprobar si el jugador seleccionado puede moverse a cada una de ellas, teniendo en cuenta su rango, tipo de movimiento y obstáculos en el camino.
        for (int j = 0; j < 9; j++) 
        {
            if (i == gx && j == gy) continue;
            if (equipoEn(i, j) == miEquipo) continue;
            if (!jugadorSeleccionado->esMovimientoValido(gx, gy, i, j)) continue;

            if (jugadorSeleccionado->esTeleport() || jugadorSeleccionado->esVolador()) {
                // Piezas voladoras y teletransportadoras no se bloquean por piezas intermedias
                casillasValidas.at(i, j) = true;
            } else 
            {
                // Terrestre: verificar que el camino este libre de obstaculos
                int difX = i - gx;
                int difY = j - gy;
                int absDifX = std::abs(difX);
                int absDifY = std::abs(difY);

                // Movimiento en L (Caballero/Goblin): salta sobre piezas intermedias
                if ((absDifX == 2 && absDifY == 1) || (absDifX == 1 && absDifY == 2)) 
                {
                    casillasValidas.at(i, j) = true;
                    continue;
                }
                // Movimiento en linea recta (ortogonal o diagonal): camino debe estar libre
                int stepX = (difX == 0) ? 0 : (difX > 0 ? 1 : -1);
                int stepY = (difY == 0) ? 0 : (difY > 0 ? 1 : -1);
                bool caminoLibre = true;
                int cx = gx + stepX;
                int cy = gy + stepY;
                while (cx != i || cy != j) {
                    if (equipoEn(cx, cy) != 0) { caminoLibre = false; break; }
                    cx += stepX;
                    cy += stepY;
                }
                if (caminoLibre) casillasValidas.at(i, j) = true;
            }
        }
    }
}

void Mundo::mueve() {}

void Mundo::tecla(unsigned char key)
{
    if (jugadorSeleccionado == nullptr) return;
    if (!jugadorSeleccionado->esEntrenador()) return;

    if (key == '8') { mostrarHechizos = !mostrarHechizos; return; }

    if (key >= '1' && key <= '7') {
        int idx = key - '1';
        Entrenador* ent = static_cast<Entrenador*>(jugadorSeleccionado);

        if (!ent->tieneConjuros()) return;
        if (ent->conjuroUsado(idx)) return;

        if (idx == 0) {
            faseConjuro = FaseConjuro::TELEPORT_PIEZA;
            equipoConjuro = ent->getEquipo();
            jugadorSeleccionado = nullptr;
            calcularCasillasValidas();
            return;
        }
        if (idx == 3) {
            faseConjuro = FaseConjuro::EXCHANGE_PRIMERA;
            equipoConjuro = ent->getEquipo();
            piezaConjuro = nullptr;
            jugadorSeleccionado = nullptr;
            calcularCasillasValidas();
            return;
        }

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
        faseConjuro = FaseConjuro::NINGUNO;
        piezaConjuro = nullptr;
        calcularCasillasValidas();
        return;
    }

    if (faseConjuro == FaseConjuro::TELEPORT_PIEZA) {
        Jugador* pj = equipoPorId(equipoConjuro).piezaEn(gx, gy);
        if (pj != nullptr) {
            piezaConjuro = pj;
            faseConjuro  = FaseConjuro::TELEPORT_DESTINO;
            for (int i = 0; i < 9; i++)
                for (int j = 0; j < 9; j++)
                    casillasValidas.at(i, j) = (equipoEn(i, j) == 0);
        }
        return;
    }

    if (faseConjuro == FaseConjuro::TELEPORT_DESTINO) {
        if (equipoEn(gx, gy) == 0) {
            piezaConjuro->moverA((float)gx, (float)gy);
            Jugador* entPj = equipoPorId(equipoConjuro).buscaEntrenador();
            if (entPj != nullptr) {
                Entrenador* ent = static_cast<Entrenador*>(entPj);
                ent->marcarConjuroUsado(0);
                ent->gastaConjuro();
            }
            faseConjuro  = FaseConjuro::NINGUNO;
            piezaConjuro = nullptr;
            calcularCasillasValidas();
            cierraTurno();
        }
        return;
    }

    if (faseConjuro == FaseConjuro::EXCHANGE_PRIMERA) {
        Jugador* pj = equipoPorId(equipoConjuro).piezaEn(gx, gy);
        if (pj != nullptr) {
            piezaConjuro = pj;
            faseConjuro  = FaseConjuro::EXCHANGE_SEGUNDA;
            for (int i = 0; i < 9; i++)
                for (int j = 0; j < 9; j++) {
                    Jugador* otro = equipoPorId(equipoConjuro).piezaEn(i, j);
                    casillasValidas.at(i, j) = (otro != nullptr && otro != pj);
                }
        }
        return;
    }

    if (faseConjuro == FaseConjuro::EXCHANGE_SEGUNDA) {
        Jugador* pj = equipoPorId(equipoConjuro).piezaEn(gx, gy);
        if (pj != nullptr && pj != piezaConjuro) {
            float ax = piezaConjuro->getPosX() - 0.5f;
            float ay = piezaConjuro->getPosY() - 0.5f;
            float bx = pj->getPosX() - 0.5f;
            float by = pj->getPosY() - 0.5f;
            piezaConjuro->moverA(bx, by);
            pj->moverA(ax, ay);
            Jugador* entPj = equipoPorId(equipoConjuro).buscaEntrenador();
            if (entPj != nullptr) {
                Entrenador* ent = static_cast<Entrenador*>(entPj);
                ent->marcarConjuroUsado(3);
                ent->gastaConjuro();
            }
            faseConjuro  = FaseConjuro::NINGUNO;
            piezaConjuro = nullptr;
            calcularCasillasValidas();
            cierraTurno();
        }
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
        mostrarHechizos     = false;
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

int Mundo::ventajaCombate() const
{
    if (miTablero.casillaFavoreceLuz(destCombateX, destCombateY))    return 1;
    if (miTablero.casillaFavoreceOscuro(destCombateX, destCombateY)) return 2;
    return 0;
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
    madrid.descuentaTemporales();
    atleti.descuentaTemporales();
    curarEnPuntosDePoder();
}

bool Mundo::curarUnaPieza(int equipo)
{
    return equipoPorId(equipo).curaUna();
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
        // Las piezas situadas en un punto de poder son inmunes a los hechizos rivales:
        // no se las puede encarcelar mientras lo ocupen.
        if (pj != nullptr && !pj->esEntrenador() && !pj->estaEncarcelado()
            && !esPuntoDePoder((int)pj->getPosX(), (int)pj->getPosY())) {
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
            Jugador* elemental = (equipo == 1)
                ? static_cast<Jugador*>(new Caballero((float)nx, (float)ny))
                : static_cast<Jugador*>(new Goblin   ((float)nx, (float)ny));
            elemental->haceTemporal(4);   // elemental temporal: dura unos turnos y desaparece
            vistas[elemental] = new VistaJugador(elemental->getRutaTextura(), elemental->getPosX(), elemental->getPosY(), 0.4f);
            miEquip.anade(elemental);
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

// Las cinco casillas de poder: el centro y el centro de cada borde del tablero.
bool Mundo::esPuntoDePoder(int x, int y) const
{
    return (x == 4 && y == 4) ||   // centro
    (x == 0 && y == 4) ||   // borde izquierdo
    (x == 8 && y == 4) ||   // borde derecho
    (x == 4 && y == 0) ||   // borde inferior
    (x == 4 && y == 8);     // borde superior
}

// Cada turno, las piezas situadas en un punto de poder regeneran vida
// (en el ARCHON las casillas de poder curan mas rapido a quien las ocupa).
void Mundo::curarEnPuntosDePoder()
{
    const int REGEN = 10;   // vida que recupera por turno una pieza sobre un punto de poder
    for (Jugador* pj : madrid.getPiezas())
        if (pj != nullptr && esPuntoDePoder((int)pj->getPosX(), (int)pj->getPosY())) pj->cura(REGEN);
    for (Jugador* pj : atleti.getPiezas())
        if (pj != nullptr && esPuntoDePoder((int)pj->getPosX(), (int)pj->getPosY())) pj->cura(REGEN);
}

int Mundo::comprobarVictoria() const
{
    // Condicion 1: controlar los 5 puntos de poder
    int controlMadrid = 0;
    int controlAtleti = 0;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (esPuntoDePoder(i, j)) {
                int eq = equipoEn(i, j);
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
