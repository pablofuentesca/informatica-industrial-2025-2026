#include "pantallanombres.h"
#include "contexto.h"
#include "ETSIDI.h"
#include "freeglut.h"

static void dibujaTexto(float x, float y, const char* texto, float r, float g, float b)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    while (*texto) { glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *texto); texto++; }
}

void PantallaNombres::inicializa()
{
    campo.limpia();
    campoNombre = 0;
}

void PantallaNombres::dibuja() const
{
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
    if (ctx->modoIA)           titulo = "Escribe tu nombre (juegas con el REAL MADRID):";
    else if (campoNombre == 0) titulo = "Jugador del REAL MADRID, escribe tu nombre:";
    else                       titulo = "Jugador del ATLETICO, escribe tu nombre:";

    dibujaTexto(160.0f, 390.0f, titulo, 0.95f, 0.88f, 0.35f);
    campo.dibuja(250.0f, 285.0f);
    dibujaTexto(245.0f, 220.0f, "ENTER para confirmar    ESC para volver", 0.6f, 0.6f, 0.6f);
}

void PantallaNombres::tecla(unsigned char key)
{
    if (key == 27)              { ctx->pide(Contexto::INICIO); return; }
    if (key == 8 || key == 127) { campo.borra(); return; }
    if (key != 13)              { campo.anadeCaracter(key); return; }

    // Enter: confirma el nombre actual
    if (campoNombre == 0) {
        ctx->nombreMadrid = campo.vacio() ? "Jugador 1" : campo.valor();
        if (!ctx->modoIA) { campoNombre = 1; campo.limpia(); return; }
        ctx->nombreAtleti = "IA";
    } else {
        ctx->nombreAtleti = campo.vacio() ? "Jugador 2" : campo.valor();
    }
    ETSIDI::playMusica("sonidos/partido.wav", true);
    ctx->pide(Contexto::JUEGO);
}
