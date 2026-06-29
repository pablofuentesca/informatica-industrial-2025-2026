#include "pantallainicio.h"
#include "contexto.h"
#include "ETSIDI.h"
#include "freeglut.h"

static void dibujaTexto(float x, float y, const char* texto, float r, float g, float b)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    while (*texto) { glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *texto); texto++; }
}

PantallaInicio::~PantallaInicio()
{
    delete portada;
}

void PantallaInicio::inicializa()
{
    portada = new ETSIDI::Sprite("../bin/imagenes/portada.png", 400, 300, 800, 600);
    botones[0] = { 200, 120, 390, 185, "1 vs 1" };
    botones[1] = { 410, 120, 620, 185, "Jugador vs IA" };
}

void PantallaInicio::dibuja() const
{
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

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    botones[0].dibuja();
    botones[1].dibuja();

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
    dibujaTexto(268, 16, "Pulsa C para ver el ranking",             0.95f, 0.85f, 0.35f);
}

void PantallaInicio::tecla(unsigned char key)
{
    if (key == 'e' || key == 'E') { ctx->modoIA = false; ctx->pide(Contexto::NOMBRES); }
    if (key == 'r' || key == 'R') { ctx->pide(Contexto::REGLAS); }
    if (key == 'c' || key == 'C') { ctx->pide(Contexto::RANKING); }
}

void PantallaInicio::raton(int boton, int estado, float x, float y)
{
    if (boton != GLUT_LEFT_BUTTON || estado != GLUT_DOWN) return;
    float mx  = x * 800.0f;
    float glY = y * 600.0f;
    if (botones[0].contiene(mx, glY)) { ctx->modoIA = false; ctx->pide(Contexto::NOMBRES); }
    if (botones[1].contiene(mx, glY)) { ctx->modoIA = true;  ctx->pide(Contexto::NOMBRES); }
}
