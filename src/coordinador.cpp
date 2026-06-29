#include "coordinador.h"
#include "pantalla.h"
#include "pantallainicio.h"
#include "pantallareglas.h"
#include "pantallanombres.h"
#include "pantallajuego.h"
#include "pantallacombate.h"
#include "pantallafin.h"
#include "pantallaranking.h"
#include "freeglut.h"

Coordinador::~Coordinador()
{
    delete actual;
}

void Coordinador::inicializa()
{
    ctx.mundo.inicializa();
    ctx.ia.setMundo(&ctx.mundo);
    ctx.ia.setEquipo(2);
    cambiaPantalla(Contexto::INICIO);
}

void Coordinador::cambiaPantalla(Contexto::Id id)
{
    Pantalla* siguiente = nullptr;
    switch (id) {
    case Contexto::INICIO:siguiente = new PantallaInicio(); break;
    case Contexto::REGLAS:siguiente = new PantallaReglas(); break;
    case Contexto::NOMBRES:siguiente = new PantallaNombres(ctx.modoIA); break;
    case Contexto::JUEGO:siguiente = new PantallaJuego(); break;
    case Contexto::COMBATE:siguiente = new PantallaCombate(); break;
    case Contexto::FIN:siguiente = new PantallaFin(); break;
    case Contexto::RANKING:siguiente = new PantallaRanking(); break;
    default: return;
    }
    // borro la pantalla anterior y monto la nueva con acceso al contexto
    delete actual;
    actual = siguiente;
    actual->setContexto(&ctx);
    actual->inicializa();
}

void Coordinador::procesaCambio()
{
    // si la pantalla activa ha pedido un cambio, lo hacemos despues de que
    // termine su evento para no borrarla mientras se esta ejecutando
    if (ctx.siguiente != Contexto::NINGUNA) {
        Contexto::Id destino = ctx.siguiente;
        ctx.siguiente = Contexto::NINGUNA;
        cambiaPantalla(destino);
    }
}

void Coordinador::dibuja() const{ if (actual) actual->dibuja(); }
void Coordinador::mueve(double dt){ if (actual) actual->mueve(dt);procesaCambio(); }
void Coordinador::tecla(unsigned char k){ if (actual) actual->tecla(k);procesaCambio(); }
void Coordinador::teclaJ1(unsigned char k){ if (actual) actual->teclaArriba(k);procesaCambio(); }
void Coordinador::teclaEspecial(int k){ if (actual) actual->teclaEspecial(k);procesaCambio(); }
void Coordinador::teclaEspecialArriba(int k){ if (actual) actual->teclaEspecialArriba(k);procesaCambio(); }

void Coordinador::raton(int boton, int estadoRat, int x, int y)
{
    // normalizo el clic al tamaño real de la ventana (0..1) para que funcione
    // tambien con la ventana maximizada, no solo en 800x600
    float ancho = (float)glutGet(GLUT_WINDOW_WIDTH);
    float alto = (float)glutGet(GLUT_WINDOW_HEIGHT);
    if (ancho <= 0.0f) ancho = 800.0f;
    if (alto <= 0.0f) alto = 600.0f;

    if (actual) actual->raton(boton, estadoRat, x / ancho, 1.0f - y / alto);
    procesaCambio();
}
