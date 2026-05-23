#include "coordinador.h"
#include "freeglut.h"

// Función para dibujar texto sin depender de archivos .ttf 
void dibujaTexto(float x, float y, const char* texto, float r, float g, float b) {
    glColor3f(r, g, b); // Color del texto
    glRasterPos2f(x, y); // Posición en la pantalla
    while (*texto) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *texto);
        texto++;
    }
}


Coordinador::Coordinador() {
    estado = INICIO;
}

Coordinador::~Coordinador() {}

void Coordinador::dibuja() {
    if (estado == INICIO) {
        // Configuramos la cámara (800x600 píxeles reales)
        glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600);
        glMatrixMode(GL_MODELVIEW); glLoadIdentity();

        // Apagamos luces y texturas por si ETSIDI las dejó encendidas
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);

        // Fondo azul oscuro futbolero
        glClearColor(0.05f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Pintamos el texto usando nuestra función nativa centrada
        dibujaTexto(235, 400, "REAL MADRID VS ATLETICO DE MADRID", 1.0f, 1.0f, 1.0f); // Blanco
        dibujaTexto(330, 350, "EDICION ARCHON", 0.8f, 1.0f, 0.6f);                    // Verde lima
        dibujaTexto(190, 200, "Pulsa la tecla 'E' para empezar el partido", 0.6f, 0.8f, 1.0f); // Azulito
    }
    else if (estado == JUEGO) {
        // Preparamos el fondo negro y LIMPIAMOS LA PANTALLA INMEDIATAMENTE
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Configuramos la cámara original del tablero
        glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(-1.0, 10.0, -1.0, 10.0);
        glMatrixMode(GL_MODELVIEW); glLoadIdentity();

        mundo.dibuja();
    }
}

void Coordinador::tecla(unsigned char key) {
    if (estado == INICIO) {
        if (key == 'e' || key == 'E') {
            mundo.inicializa();
            estado = JUEGO;
        }
    }
    else if (estado == JUEGO) {
        mundo.tecla(key);
    }
}

void Coordinador::teclaEspecial(int key) {
    if (estado == JUEGO) {
        mundo.teclaEspecial(key);
    }
}