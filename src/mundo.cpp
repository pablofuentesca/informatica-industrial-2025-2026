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
    if (jugadorSeleccionado != nullptr) {
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f, 0.0f, 0.0f); // Rojo puro
        glLineWidth(3.0f); // Hacemos la línea más gruesa para que destaque

        int actualX = int(jugadorSeleccionado->getPosX());
        int actualY = int(jugadorSeleccionado->getPosY());

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                // Le preguntamos a la pieza: "¿Tus reglas te dejan ir aquí?"
                if (jugadorSeleccionado->esMovimientoValido(actualX, actualY, i, j)) {
                    float cx = i + 0.5f;
                    float cy = j + 0.5f;
                    float r = 0.48f; // Un pelín más pequeño que la casilla

                    glBegin(GL_LINE_LOOP); // Dibuja solo el marco
                    glVertex2f(cx - r, cy - r);
                    glVertex2f(cx + r, cy - r);
                    glVertex2f(cx + r, cy + r);
                    glVertex2f(cx - r, cy + r);
                    glEnd();
                }
            }
        }
        glEnable(GL_TEXTURE_2D);
        glLineWidth(1.0f); // Restauramos el grosor normal de OpenGL
    }
}

void Mundo::mueve() {}

void Mundo::tecla(unsigned char key) {}

void Mundo::teclaEspecial(int key) {}

void Mundo::raton(int boton, int estado, float x, float y) {
    if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {

        Jugador* tocado = nullptr;

        //Comprobar Madrid usando GETTERS
        for (int i = 0; i < 18; i++) {
            if (equipoMadrid[i] != nullptr) {
                float difX = x - equipoMadrid[i]->getPosX();
                float difY = y - equipoMadrid[i]->getPosY();
                if (difX > -0.5f && difX < 0.5f && difY > -0.5f && difY < 0.5f) tocado = equipoMadrid[i];
            }
        }

        //Comprobar Atleti usando GETTERS
        for (int i = 0; i < 18; i++) {
            if (equipoAtleti[i] != nullptr) {
                float difX = x - equipoAtleti[i]->getPosX();
                float difY = y - equipoAtleti[i]->getPosY();
                if (difX > -0.5f && difX < 0.5f && difY > -0.5f && difY < 0.5f) tocado = equipoAtleti[i];
            }
        }

        //Lógica de juego usando GETTERS Y SETTERS
        if (jugadorSeleccionado == nullptr) {
            jugadorSeleccionado = tocado;
        }
        else {
            if (tocado == nullptr) {
                // SEGUNDO CLIC en vacío
                int targetX = int(floor(x));
                int targetY = int(floor(y));

                int actualX = int(jugadorSeleccionado->getPosX());
                int actualY = int(jugadorSeleccionado->getPosY());

                // Comprobamos si el movimiento cumple las reglas de la pieza
                if (jugadorSeleccionado->esMovimientoValido(actualX, actualY, targetX, targetY)) {
                    float centroX = targetX + 0.5f;
                    float centroY = targetY + 0.5f;
                    jugadorSeleccionado->setPosicion(centroX, centroY);
                    jugadorSeleccionado = nullptr; // Soltamos al jugador tras moverlo con éxito
                }
                // Si la casilla no es válida, la condición no se cumple y el juego 
                // no hace absolutamente nada. El jugador seguirá seleccionado.
            }
        }
    }
}