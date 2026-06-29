#include "pantallajuego.h"
#include "contexto.h"
#include "freeglut.h"

static void iniciaCombate(Contexto* ctx)
{
    ctx->arena.inicializa(ctx->mundo.getCombatiente1(), ctx->mundo.getCombatiente2());
    ctx->arena.setVentajaCasilla(ctx->mundo.ventajaCombate());
    if (ctx->modoIA) ctx->arena.setIAActiva(true);
    ctx->pide(Contexto::COMBATE);
}

void PantallaJuego::dibuja() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(-1.0, 10.0, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);  glLoadIdentity();
    ctx->mundo.dibuja();
}

void PantallaJuego::mueve(double dt)
{
    int v = ctx->mundo.comprobarVictoria();
    if (v != 0) {
        ctx->equipoVencedor = v;
        ctx->ranking.registraVictoria(v == 1 ? ctx->nombreMadrid : ctx->nombreAtleti);
        ctx->pide(Contexto::FIN);
        return;
    }

    if (ctx->modoIA && ctx->ia.mueve(dt) && ctx->mundo.hayCombatePendiente())
        iniciaCombate(ctx);
}

void PantallaJuego::tecla(unsigned char key)    { ctx->mundo.tecla(key); }
void PantallaJuego::teclaEspecial(int key)       { ctx->mundo.teclaEspecial(key); }

void PantallaJuego::raton(int boton, int estado, float x, float y)
{
    ctx->mundo.raton(boton, estado, x * 11.0f - 1.0f, y * 11.0f - 1.0f);

    if (ctx->mundo.hayCombatePendiente()) { iniciaCombate(ctx); return; }
    if (ctx->modoIA) ctx->ia.elegirMovimiento();
}
