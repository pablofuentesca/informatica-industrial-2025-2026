#include "pantallareglas.h"
#include "contexto.h"
#include "freeglut.h"

// fuente grande para titulos y avisos
static void dibujaTexto(float x, float y, const char* texto, float r, float g, float b)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    while (*texto) { glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *texto); texto++; }
}

// fuente pequena para tablas y contenido denso
static void dibujaTextoS(float x, float y, const char* texto, float r, float g, float b)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    while (*texto) { glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *texto); texto++; }
}

void PantallaReglas::dibuja() const
{
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

    dibujaTexto(113, 551, "REGLAS DEL JUEGO — ARCHON: REAL MADRID vs ATLETICO", 0.9f, 0.75f, 0.1f);

    auto sep = [](float sy) {
        glColor3f(0.45f, 0.35f, 0.05f);
        glBegin(GL_LINES);
            glVertex2f(30, sy); glVertex2f(770, sy);
        glEnd();
    };

    // CONTROLES 
    dibujaTextoS( 35, 534, "CONTROLES", 1.0f, 1.0f, 0.5f);
    dibujaTextoS( 35, 519, "  J1 (Madrid):  W A S D para moverse            ESPACIO para atacar", 0.8f, 0.8f, 0.8f);
    dibujaTextoS( 35, 504, "  J2 (Atleti):  Flechas cursor para moverse      INTRO para atacar", 0.8f, 0.8f, 0.8f);
    dibujaTextoS( 35, 489, "  Tablero: clic izquierdo selecciona pieza / mueve / lanza combate", 0.8f, 0.8f, 0.8f);
    dibujaTextoS( 35, 474, "  ESC=volver  P=pausa arena  R=reglas  C=ranking  0=combate prueba  1-7=conjuro", 0.8f, 0.8f, 0.8f);
    sep(463.0f);

    //VICTORIA 
    dibujaTextoS( 35, 450, "VICTORIA", 1.0f, 1.0f, 0.5f);
    dibujaTextoS( 35, 435, "  Controla los 5 puntos de poder (casilla central + 4 esquinas)  O  elimina al Entrenador rival", 0.8f, 0.8f, 0.8f);
    dibujaTextoS( 35, 420, "  Ciclo de luz (0-6): casillas claras favorecen a Madrid; casillas oscuras favorecen a Atletico", 0.8f, 0.8f, 0.8f);
    sep(409.0f);

    // CONJUROS
    dibujaTextoS( 35, 396, "CONJUROS — seleccionar Entrenador + pulsar 1-7  (7 cargas/partida, 1 uso cada una)", 1.0f, 1.0f, 0.5f);
    dibujaTextoS( 35, 381, "  [1] Teleport:     mueve pieza aliada a casilla libre aleatoria", 0.8f, 0.8f, 0.8f);
    dibujaTextoS( 35, 366, "  [2] Curar:        cura completamente a la primera pieza aliada herida", 0.8f, 0.8f, 0.8f);
    dibujaTextoS( 35, 351, "  [3] Cambio ciclo: invierte la direccion del ciclo de luz del tablero", 0.8f, 0.8f, 0.8f);
    dibujaTextoS( 35, 336, "  [4] Intercambiar: cambia la posicion de dos piezas aliadas", 0.8f, 0.8f, 0.8f);
    dibujaTextoS(415, 381, "  [5] Elemental:    invoca elemental temporal junto al Entrenador", 0.8f, 0.8f, 0.8f);
    dibujaTextoS(415, 366, "  [6] Revivir:      resucita la ultima pieza eliminada del equipo", 0.8f, 0.8f, 0.8f);
    dibujaTextoS(415, 351, "  [7] Encarcelar:   inmoviliza una pieza enemiga durante 3 turnos", 0.8f, 0.8f, 0.8f);
    sep(325.0f);

    // PIEZAS
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

    //ARENA
    dibujaTextoS( 35, yf - 38.0f, "ARENA DE COMBATE", 1.0f, 1.0f, 0.5f);
    dibujaTextoS( 35, yf - 53.0f, "  6 barreras + 4 charcos al inicio. Nadie puede moverse hasta que esten todos.", 0.8f, 0.8f, 0.8f);
    dibujaTextoS( 35, yf - 68.0f, "  Barreras: bloquean movimiento y disparos (Banshee los atraviesa). Charcos: frenan el movimiento.", 0.8f, 0.8f, 0.8f);
    dibujaTextoS( 35, yf - 83.0f, "  Los voladores (vuela) pasan por encima de todos los obstaculos sin ser frenados.", 0.8f, 0.8f, 0.8f);

    dibujaTextoS(280, 35, "Pulsa ESC para volver al menu", 0.55f, 0.55f, 0.55f);
}

void PantallaReglas::tecla(unsigned char key)
{
    if (key == 27) ctx->pide(Contexto::INICIO);
}
