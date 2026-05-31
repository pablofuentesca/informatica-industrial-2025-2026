#include "coordinador.h"
#include "ETSIDI.h"
#include "freeglut.h"

Coordinador::~Coordinador()
{
    if (portada != nullptr) delete portada;
}

void Coordinador::inicializa()
{
    portada = new ETSIDI::Sprite("../bin/imagenes/portada.png", 400, 300, 800, 600);
    mundo.inicializa();
    ia.setMundo(&mundo);
    ia.setEquipo(2);
}

static void dibujaTexto(float x, float y, const char* texto, float r, float g, float b)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    while (*texto) { glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *texto); texto++; }
}

// fuente pequeña para tablas y contenido denso
static void dibujaTextoS(float x, float y, const char* texto, float r, float g, float b)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    while (*texto) { glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *texto); texto++; }
}

static void dibujaBoton(float x1, float y1, float x2, float y2,
                         const char* texto, bool activo)
{
    // Fondo oscuro semitransparente
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.05f, 0.03f, 0.0f, 0.85f);
    glBegin(GL_QUADS);
        glVertex2f(x1, y1); glVertex2f(x2, y1);
        glVertex2f(x2, y2); glVertex2f(x1, y2);
    glEnd();
    glDisable(GL_BLEND);

    // Borde dorado (grisáceo si inactivo)
    glLineWidth(2.5f);
    glColor3f(activo ? 0.9f : 0.45f, activo ? 0.7f : 0.4f, activo ? 0.1f : 0.1f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x1, y1); glVertex2f(x2, y1);
        glVertex2f(x2, y2); glVertex2f(x1, y2);
    glEnd();
    glLineWidth(1.0f);

    // Texto centrado
    float textW = (float)glutBitmapLength(GLUT_BITMAP_HELVETICA_18,
                                          (const unsigned char*)texto);
    float cx = x1 + (x2 - x1 - textW) / 2.0f;
    float cy = y1 + (y2 - y1) / 2.0f - 6.0f;
    glColor3f(activo ? 1.0f : 0.5f, activo ? 0.85f : 0.5f, activo ? 0.4f : 0.4f);
    glRasterPos2f(cx, cy);
    for (const char* c = texto; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void Coordinador::iniciaEntradaNombres(bool conIA)
{
    modoIA        = conIA;
    nombreMadrid  = "";
    nombreAtleti  = conIA ? "IA" : "";   // contra la maquina el rival es la IA
    entradaActual = "";
    campoNombre   = 0;                    // empezamos pidiendo el nombre de Madrid
    estado        = NOMBRES;
}

void Coordinador::mueve(double dt)
{
    if (estado == COMBATE) {
        arena.mueve(dt);
        if (arena.getGanador() != 0) {
            ETSIDI::stopMusica();
            mundo.resolverCombate(arena.getGanador());
            arena.inicializa();
            int v = mundo.comprobarVictoria();
            if (v != 0) {
                equipoVencedor = v;
                estado = FIN;
                ranking.registraVictoria(v == 1 ? nombreMadrid : nombreAtleti);
            }
            else {
                estado = JUEGO;
                ETSIDI::playMusica("sonidos/partido.wav", true);
                if (modoIA) ia.elegirMovimiento();
            }
        }
    }
    if (estado == JUEGO) {
        int v = mundo.comprobarVictoria();
        if (v != 0) {
            equipoVencedor = v;
            estado = FIN;
            ranking.registraVictoria(v == 1 ? nombreMadrid : nombreAtleti);
        }

        if (modoIA) {
            if (ia.mueve(dt) && mundo.hayCombatePendiente()) {
                arena.inicializa(mundo.getCombatiente1(), mundo.getCombatiente2());
                arena.setVentajaCasilla(mundo.ventajaCombate());
                arena.setIAActiva(true);
                estado = COMBATE;
            }
        }
    }
}

void Coordinador::dibuja() const
{
    switch (estado) {
    case INICIO:
        glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600);
        glMatrixMode(GL_MODELVIEW);  glLoadIdentity();
        glDisable(GL_LIGHTING);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (portada != nullptr) {
            glEnable(GL_TEXTURE_2D);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            portada->draw();
            glDisable(GL_TEXTURE_2D);
        }

        // Resetear estados que el sprite puede haber dejado sucios
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        dibujaBoton(200, 120, 390, 185, "1 vs 1",       true);
        dibujaBoton(410, 120, 620, 185, "Jugador vs IA", true);

        // franja oscura inferior para que los textos de ayuda se lean bien
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
        glBegin(GL_QUADS);
            glVertex2f(0, 6); glVertex2f(800, 6);
            glVertex2f(800, 64); glVertex2f(0, 64);
        glEnd();
        glDisable(GL_BLEND);

        dibujaTexto(232, 40, "Pulsa R para ver las reglas del combate", 0.95f, 0.85f, 0.35f);
        dibujaTexto(268, 16, "Pulsa C para ver el ranking", 0.95f, 0.85f, 0.35f);
        break;

    case REGLAS: {
        glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600);
        glMatrixMode(GL_MODELVIEW);  glLoadIdentity();
        glDisable(GL_LIGHTING);
        glClearColor(0.02f, 0.04f, 0.02f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // fondo oscuro con borde dorado
        glColor3f(0.03f, 0.06f, 0.03f);
        glBegin(GL_QUADS);
            glVertex2f(30, 30); glVertex2f(770, 30);
            glVertex2f(770, 570); glVertex2f(30, 570);
        glEnd();
        glColor3f(0.7f, 0.55f, 0.05f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(30, 30); glVertex2f(770, 30);
            glVertex2f(770, 570); glVertex2f(30, 570);
        glEnd();
        glLineWidth(1.0f);

        // titulo
        dibujaTexto(113, 551, "REGLAS DEL JUEGO — ARCHON: REAL MADRID vs ATLETICO", 0.9f, 0.75f, 0.1f);

        {
            // separador horizontal dorado
            auto sep = [](float sy) {
                glColor3f(0.45f, 0.35f, 0.05f);
                glBegin(GL_LINES);
                    glVertex2f(30, sy); glVertex2f(770, sy);
                glEnd();
            };

            // ── CONTROLES ──────────────────────────────────────────────────
            dibujaTextoS( 35, 534, "CONTROLES", 1.0f, 1.0f, 0.5f);
            dibujaTextoS( 35, 519, "  J1 (Madrid):  W A S D para moverse            ESPACIO para atacar", 0.8f, 0.8f, 0.8f);
            dibujaTextoS( 35, 504, "  J2 (Atleti):  Flechas cursor para moverse      INTRO para atacar", 0.8f, 0.8f, 0.8f);
            dibujaTextoS( 35, 489, "  Tablero: clic izquierdo selecciona pieza / mueve / lanza combate", 0.8f, 0.8f, 0.8f);
            dibujaTextoS( 35, 474, "  ESC=volver  P=pausa arena  R=reglas  C=ranking  0=combate prueba  1-7=conjuro", 0.8f, 0.8f, 0.8f);
            sep(463.0f);

            // ── VICTORIA ───────────────────────────────────────────────────
            dibujaTextoS( 35, 450, "VICTORIA", 1.0f, 1.0f, 0.5f);
            dibujaTextoS( 35, 435, "  Controla los 5 puntos de poder (casilla central + 4 esquinas)  O  elimina al Entrenador rival", 0.8f, 0.8f, 0.8f);
            dibujaTextoS( 35, 420, "  Ciclo de luz (0-6): casillas claras favorecen a Madrid; casillas oscuras favorecen a Atletico", 0.8f, 0.8f, 0.8f);
            sep(409.0f);

            // ── CONJUROS ───────────────────────────────────────────────────
            dibujaTextoS( 35, 396, "CONJUROS — seleccionar Entrenador + pulsar 1-7  (7 cargas/partida, 1 uso cada una)", 1.0f, 1.0f, 0.5f);
            dibujaTextoS( 35, 381, "  [1] Teleport:     mueve pieza aliada a casilla libre aleatoria", 0.8f, 0.8f, 0.8f);
            dibujaTextoS( 35, 366, "  [2] Curar:        cura completamente a la primera pieza aliada herida", 0.8f, 0.8f, 0.8f);
            dibujaTextoS( 35, 351, "  [3] Cambio ciclo: invierte la direccion del ciclo de luz del tablero", 0.8f, 0.8f, 0.8f);
            dibujaTextoS( 35, 336, "  [4] Intercambiar: cambia la posicion de dos piezas aliadas", 0.8f, 0.8f, 0.8f);
            dibujaTextoS(415, 381, "  [5] Elemental:    invoca elemental temporal junto al Entrenador", 0.8f, 0.8f, 0.8f);
            dibujaTextoS(415, 366, "  [6] Revivir:      resucita la ultima pieza eliminada del equipo", 0.8f, 0.8f, 0.8f);
            dibujaTextoS(415, 351, "  [7] Encarcelar:   inmoviliza una pieza enemiga durante 3 turnos", 0.8f, 0.8f, 0.8f);
            sep(325.0f);

            // ── PIEZAS ─────────────────────────────────────────────────────
            dibujaTextoS( 35, 313, "PIEZAS DE COMBATE  (HP / vel px/s / dmg / cd s / notas)", 1.0f, 1.0f, 0.5f);
            dibujaTextoS( 35, 298, "REAL MADRID", 0.85f, 0.85f, 1.0f);
            dibujaTextoS(415, 298, "ATLETICO",    1.0f,  0.65f, 0.65f);

            struct Fila { const char* mad; const char* atl; };
            static const Fila filas[] = {
                { "Courtois  (Arq.)  30  200  12  0.25",  "Morata    (Man.)  70  180  18  0.80  triple x3"   },
                { "Mbappe    (Cab.) 100  180  25  0.35  mele",  "Goblin    (Inf.)  25  210   8  0.50  enjambre x3" },
                { "Militao   (Gol.)  75  130  20  0.80  mele",  "Savic     (Trol)  90  130  22  0.90  mele +5HP/s" },
                { "Bellingham(Uni.)  60  220  15  0.60",         "Koke      (Bas.)  55  150  12  0.70  paraliza 1s" },
                { "Vinicius  (Val.)  70  240  18  0.60  mele vuela", "DePaul   (Ban.)  65  240  16  0.70  vuela trv."  },
                { "Valverde  (Gen.)  70  200  15  0.80  vuela", "Correa    (Cam.)  70  200  15  0.80  vuela"       },
                { "Carvajal  (Fen.)  55  220  17  0.40  vuela revive", "Griezmann(Dra.) 100  124   2  0.21  vuela chorro"},
                { "Ancelotti(Mag.*)  40  130  12  1.00  rayo vuela",  "Simeone  (Hec.*)  40  130  12  1.00  rayo vuela"  },
            };
            float yf = 283.0f;
            for (const auto& f : filas) {
                dibujaTextoS( 35, yf, f.mad, 0.75f, 0.75f, 0.75f);
                dibujaTextoS(415, yf, f.atl, 0.75f, 0.75f, 0.75f);
                yf -= 15.0f;
            }
            dibujaTextoS(35, yf,        "(*) PIEZA REY: si muere en arena, ese equipo pierde la partida", 0.65f, 0.60f, 0.2f);
            dibujaTextoS(35, yf - 14.0f,"mele=cuerpo a cuerpo  vuela=vuela sobre piezas y obstaculos  trv=atraviesa barreras", 0.55f, 0.55f, 0.55f);
            sep(yf - 25.0f);

            // ── ARENA ──────────────────────────────────────────────────────
            dibujaTextoS( 35, yf - 38.0f, "ARENA DE COMBATE", 1.0f, 1.0f, 0.5f);
            dibujaTextoS( 35, yf - 53.0f, "  6 barreras + 4 charcos al inicio. Nadie puede moverse hasta que esten todos.", 0.8f, 0.8f, 0.8f);
            dibujaTextoS( 35, yf - 68.0f, "  Barreras: bloquean movimiento y disparos (Banshee los atraviesa). Charcos: frenan el movimiento.", 0.8f, 0.8f, 0.8f);
            dibujaTextoS( 35, yf - 83.0f, "  Los voladores (vuela) pasan por encima de todos los obstaculos sin ser frenados.", 0.8f, 0.8f, 0.8f);
        }

        dibujaTextoS(280, 35, "Pulsa ESC para volver al menu", 0.55f, 0.55f, 0.55f);
        break;
    }

    case JUEGO:
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(-1.0, 10.0, -1.0, 10.0);
        glMatrixMode(GL_MODELVIEW);  glLoadIdentity();
        mundo.dibuja();
        break;

    case COMBATE:
        glClear(GL_COLOR_BUFFER_BIT);
        arena.dibuja();
        break;

    case FIN: {
        glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600);
        glMatrixMode(GL_MODELVIEW);  glLoadIdentity();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);

        std::string ganador = "GANA " + (equipoVencedor == 1 ? nombreMadrid : nombreAtleti);
        dibujaTexto(250.0f, 330.0f, ganador.c_str(), 1.0f, 0.85f, 0.1f);
        dibujaTexto(270.0f, 285.0f, "Pulsa ESC para volver al inicio", 0.6f, 0.6f, 0.6f);
        dibujaTexto(270.0f, 255.0f, "Pulsa C para ver el ranking", 0.6f, 0.6f, 0.6f);
        break;
    }

    case RANKING:
        glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600);
        glMatrixMode(GL_MODELVIEW);  glLoadIdentity();
        glDisable(GL_LIGHTING);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ranking.dibuja();
        break;

    case NOMBRES: {
        glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600);
        glMatrixMode(GL_MODELVIEW);  glLoadIdentity();
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glClearColor(0.02f, 0.03f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // franja superior con los colores de los dos equipos (blanco | rojo)
        glColor3f(0.90f, 0.90f, 0.95f);
        glBegin(GL_QUADS);
            glVertex2f(0, 560); glVertex2f(400, 560);
            glVertex2f(400, 600); glVertex2f(0, 600);
        glEnd();
        glColor3f(0.70f, 0.10f, 0.15f);
        glBegin(GL_QUADS);
            glVertex2f(400, 560); glVertex2f(800, 560);
            glVertex2f(800, 600); glVertex2f(400, 600);
        glEnd();

        // panel central con borde dorado
        glColor3f(0.05f, 0.07f, 0.10f);
        glBegin(GL_QUADS);
            glVertex2f(120, 180); glVertex2f(680, 180);
            glVertex2f(680, 430); glVertex2f(120, 430);
        glEnd();
        glColor3f(0.80f, 0.65f, 0.10f);
        glLineWidth(2.5f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(120, 180); glVertex2f(680, 180);
            glVertex2f(680, 430); glVertex2f(120, 430);
        glEnd();
        glLineWidth(1.0f);

        const char* titulo;
        if (modoIA)                titulo = "Escribe tu nombre (juegas con el REAL MADRID):";
        else if (campoNombre == 0) titulo = "Jugador del REAL MADRID, escribe tu nombre:";
        else                       titulo = "Jugador del ATLETICO, escribe tu nombre:";

        dibujaTexto(160.0f, 390.0f, titulo, 0.95f, 0.88f, 0.35f);

        // caja de texto donde aparece lo que se escribe
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
            glVertex2f(250, 285); glVertex2f(550, 285);
            glVertex2f(550, 330); glVertex2f(250, 330);
        glEnd();
        glColor3f(0.50f, 0.50f, 0.55f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(250, 285); glVertex2f(550, 285);
            glVertex2f(550, 330); glVertex2f(250, 330);
        glEnd();

        std::string linea = entradaActual + "_";   // el subrayado hace de cursor
        dibujaTexto(265.0f, 300.0f, linea.c_str(), 1.0f, 1.0f, 1.0f);

        dibujaTexto(245.0f, 220.0f, "ENTER para confirmar    ESC para volver", 0.6f, 0.6f, 0.6f);
        break;
    }

    default:
        break;
    }
}

void Coordinador::tecla(unsigned char key)
{
    switch (estado) {
    case INICIO:
        if (key == 'e' || key == 'E')   { iniciaEntradaNombres(false); }
        if (key == 'r' || key == 'R')   { estado = REGLAS; }
        if (key == 'c' || key == 'C')   { estado = RANKING; }
        break;
    case REGLAS:
        if (key == 27) { estado = INICIO; }
        break;
    case RANKING:
        if (key == 27) { estado = INICIO; }
        break;
    case NOMBRES:
        if (key == 27) { estado = INICIO; break; }          // ESC cancela
        if (key == 8 || key == 127) {                       // borrar un caracter
            if (!entradaActual.empty()) entradaActual.pop_back();
            break;
        }
        if (key == 13) {                                    // ENTER confirma el nombre
            if (campoNombre == 0) {
                nombreMadrid = entradaActual.empty() ? "Jugador 1" : entradaActual;
                if (modoIA) { estado = JUEGO; ETSIDI::playMusica("sonidos/partido.wav", true); }   // el rival ya es la IA
                else { campoNombre = 1; entradaActual = ""; }   // pedir el de Atleti
            }
            else {
                nombreAtleti = entradaActual.empty() ? "Jugador 2" : entradaActual;
                estado = JUEGO;
                ETSIDI::playMusica("sonidos/partido.wav", true);
            }
            break;
        }
        if (key >= 32 && key < 127 && entradaActual.size() < 12)   // caracter normal
            entradaActual += (char)key;
        break;
    case JUEGO:
        mundo.tecla(key);
        break;
    case FIN:
        if (key == 27) { equipoVencedor = 0; estado = INICIO; }
        if (key == 'c' || key == 'C') { estado = RANKING; }
        break;
    case COMBATE:
        if (key == 27) {
            ETSIDI::stopMusica();
            if (arena.getGanador() != 0)
                mundo.resolverCombate(arena.getGanador());
            else
                mundo.limpiarCombatePendiente();
            arena.inicializa();   // limpia punteros internos de arena
            estado = JUEGO;
            ETSIDI::playMusica("sonidos/partido.wav", true);
            return;
        }
        arena.tecla(key);
        break;
    default:
        break;
    }
}

void Coordinador::teclaJ1(unsigned char key)
{
    if (estado == COMBATE) arena.teclaJ1(key);
}

void Coordinador::teclaEspecial(int key)
{
    switch (estado) {
    case JUEGO:   mundo.teclaEspecial(key); break;
    case COMBATE: arena.teclaEspecial(key); break;
    default:      break;
    }
}

void Coordinador::teclaEspecialArriba(int key)
{
    if (estado == COMBATE) arena.teclaEspecialArriba(key);
}

void Coordinador::raton(int boton, int estadoRat, int x, int y)
{
	//Aqui esta lasolucion al bug de la ventana maximizada: el clic se normaliza al tamaño real de la ventana, no a un 800x600 fijo.
    float anchoVentana = (float)glutGet(GLUT_WINDOW_WIDTH);
    float altoVentana  = (float)glutGet(GLUT_WINDOW_HEIGHT);
    if (anchoVentana <= 0.0f) anchoVentana = 800.0f;
    if (altoVentana  <= 0.0f) altoVentana  = 600.0f;

    if (estado == INICIO) {
        if (boton == GLUT_LEFT_BUTTON && estadoRat == GLUT_DOWN) {
            // El menu se dibuja en gluOrtho2D(0,800, 0,600): reescalamos el clic
            // a ese espacio. Boton "1 vs 1": x[200-390], GLUT y[415-480].
            float mx = (x / anchoVentana) * 800.0f;
            float my = (y / altoVentana)  * 600.0f;
            if (mx >= 200 && mx <= 390 && my >= 415 && my <= 480) {
                iniciaEntradaNombres(false);
            }
            if (mx >= 410 && mx <= 620 && my >= 415 && my <= 480) {
                iniciaEntradaNombres(true);
            }
        }
        return;
    }

    if (estado == JUEGO) {
        // El tablero se dibuja en gluOrtho2D(-1,10, -1,10) → rango logico de 11 unidades.
        float x_logico = (x / anchoVentana) * 11.0f - 1.0f;
        float y_logico = ((altoVentana - y) / altoVentana) * 11.0f - 1.0f;
        mundo.raton(boton, estadoRat, x_logico, y_logico);

        if (mundo.hayCombatePendiente()) {
            arena.inicializa(mundo.getCombatiente1(), mundo.getCombatiente2());
            arena.setVentajaCasilla(mundo.ventajaCombate());
            if (modoIA) arena.setIAActiva(true);
            estado = COMBATE;
        }

        if (modoIA && estado == JUEGO) ia.elegirMovimiento();
    }
}
