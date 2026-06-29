#include "pantallacombate.h"
#include "contexto.h"
#include "ETSIDI.h"
#include "freeglut.h"

void PantallaCombate::dibuja() const
{
    glClear(GL_COLOR_BUFFER_BIT);
    ctx->arena.dibuja();
}

void PantallaCombate::mueve(double dt)
{
    ctx->arena.mueve(dt);

    // la batalla ha terminado: trasladamos el resultado al tablero
    if (ctx->arena.getGanador() != 0) {
        ETSIDI::stopMusica();
        ctx->mundo.resolverCombate(ctx->arena.getGanador());
        ctx->arena.inicializa();   // limpia los punteros internos de la arena

        int v = ctx->mundo.comprobarVictoria();
        if (v != 0) {
            ctx->equipoVencedor = v;
            ctx->ranking.registraVictoria(v == 1 ? ctx->nombreMadrid : ctx->nombreAtleti);
            ctx->pide(Contexto::FIN);
        }
        else {
            ETSIDI::playMusica("sonidos/partido.wav", true);
            if (ctx->modoIA) ctx->ia.elegirMovimiento();
            ctx->pide(Contexto::JUEGO);
        }
    }
}

void PantallaCombate::tecla(unsigned char key)
{
    // ESC abandona el combate y vuelve al tablero sin esperar a que haya ganador
    if (key == 27) {
        ETSIDI::stopMusica();
        if (ctx->arena.getGanador() != 0)
            ctx->mundo.resolverCombate(ctx->arena.getGanador());
        else
            ctx->mundo.limpiarCombatePendiente();
        ctx->arena.inicializa();
        ETSIDI::playMusica("sonidos/partido.wav", true);
        ctx->pide(Contexto::JUEGO);
        return;
    }
    ctx->arena.tecla(key);
}

void PantallaCombate::teclaArriba(unsigned char key) { ctx->arena.teclaJ1(key); }
void PantallaCombate::teclaEspecial(int key)         { ctx->arena.teclaEspecial(key); }
void PantallaCombate::teclaEspecialArriba(int key)   { ctx->arena.teclaEspecialArriba(key); }
