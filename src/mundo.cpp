#include "mundo.h"
#include "portero.h"
#include "delantero.h"
#include "central.h"
#include "lateral.h"
#include "centrocampista.h"
#include "mediapunta.h"
#include "extremo.h"
#include "entrenador.h"
#include "freeglut.h"

Mundo::Mundo() : balones{ Pelota(4, 4), Pelota(0, 4), Pelota(8, 4), Pelota(4, 0), Pelota(4, 8) }
{
    jugadorSeleccionado = nullptr;
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
}

void Mundo::dibuja() const
{
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    miTablero.dibuja();

    glEnable(GL_TEXTURE_2D);
    for (int i = 0; i < 5; i++) balones[i].dibuja();

    if (jugadorSeleccionado != nullptr) {
        //Esto es para que no se ponga la casillla del jugador seleccionado en negro
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING); // Apagamos las luces que dejó ETSIDI
        glDisable(GL_BLEND);    // Apagamos mezclas raras
        

        glColor3f(1.0f, 1.0f, 0.0f); //Amarillo fosforescente puro

        float radio = 0.5f;
        glBegin(GL_QUADS);
        glVertex2f(jugadorSeleccionado->pos.x - radio, jugadorSeleccionado->pos.y - radio);
        glVertex2f(jugadorSeleccionado->pos.x + radio, jugadorSeleccionado->pos.y - radio);
        glVertex2f(jugadorSeleccionado->pos.x + radio, jugadorSeleccionado->pos.y + radio);
        glVertex2f(jugadorSeleccionado->pos.x - radio, jugadorSeleccionado->pos.y + radio);
        glEnd();

        glEnable(GL_TEXTURE_2D); // Volvemos a encender texturas para las fotos de los jugadores
    }

    glDisable(GL_TEXTURE_2D);
    for (int i = 0; i < 18; i++) {
        if (equipoMadrid[i] != nullptr) equipoMadrid[i]->dibuja();
        if (equipoAtleti[i] != nullptr) equipoAtleti[i]->dibuja();
    }
}

void Mundo::mueve() {}

void Mundo::tecla(unsigned char key) {}

void Mundo::teclaEspecial(int key) {}

void Mundo::raton(int boton, int estado, float x, float y) {
    if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
        // Deseleccionamos todo primero
        jugadorSeleccionado = nullptr;

        // Comprobamos si el clic ha dado en un jugador del Madrid
        for (int i = 0; i < 18; i++) {
            if (equipoMadrid[i] != nullptr) {
                float difX = x - equipoMadrid[i]->pos.x;
                float difY = y - equipoMadrid[i]->pos.y;
                // Si la distancia al centro es menor que su radio... ¡Tocado!
                if (difX > -0.5f && difX < 0.5f && difY > -0.5f && difY < 0.5f) {
                    jugadorSeleccionado = equipoMadrid[i];
                }
            }
        }

        // Comprobamos si el clic ha dado en un jugador del Atleti
        for (int i = 0; i < 18; i++) {
            if (equipoAtleti[i] != nullptr) {
                float difX = x - equipoAtleti[i]->pos.x;
                float difY = y - equipoAtleti[i]->pos.y;
                if (difX > -0.5f && difX < 0.5f && difY > -0.5f && difY < 0.5f) {
                    jugadorSeleccionado = equipoAtleti[i];
                }
            }
        }
    }
}