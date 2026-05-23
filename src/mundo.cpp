#include "mundo.h"
#include "freeglut.h"

// En el constructor inicializamos las posiciones de los 5 balones y vaciamos las listas
Mundo::Mundo() : balones{ Pelota(4, 4), Pelota(0, 4), Pelota(8, 4), Pelota(4, 0), Pelota(4, 8) } {
    for (int i = 0; i < 18; i++) {
        equipoMadrid[i] = nullptr;
        equipoAtleti[i] = nullptr;
    }
}

// El destructor limpiará la memoria cuando cerremos el juego
Mundo::~Mundo() {
    for (int i = 0; i < 18; i++) {
        if (equipoMadrid[i] != nullptr) delete equipoMadrid[i];
        if (equipoAtleti[i] != nullptr) delete equipoAtleti[i];
    }
}

void Mundo::inicializa() {
    //REAL MADRID (Izquierda del tablero)

    // Línea frontal (Columna 1)
    equipoMadrid[0] = new Jugador(1, 0, 1, PORTERO);   // Arquera abajo
    for (int i = 1; i < 8; i++) {
        equipoMadrid[i] = new Jugador(1, i, 1, DELANTERO); // Caballeros en el medio
    }
    equipoMadrid[8] = new Jugador(1, 8, 1, PORTERO);   // Arquera arriba

    // Línea trasera (Columna 0)
    equipoMadrid[9] = new Jugador(0, 0, 1, CENTROCAMPISTA);  // Valkiria
    equipoMadrid[10] = new Jugador(0, 1, 1, CENTRAL);        // Golem
    equipoMadrid[11] = new Jugador(0, 2, 1, LATERAL);        // Unicornio
    equipoMadrid[12] = new Jugador(0, 3, 1, MEDIAPUNTA);     // Genio
    equipoMadrid[13] = new Jugador(0, 4, 1, ENTRENADOR);     // Mago (Centro exacto)
    equipoMadrid[14] = new Jugador(0, 5, 1, EXTREMO);        // Fénix
    equipoMadrid[15] = new Jugador(0, 6, 1, LATERAL);        // Unicornio
    equipoMadrid[16] = new Jugador(0, 7, 1, CENTRAL);        // Golem
    equipoMadrid[17] = new Jugador(0, 8, 1, CENTROCAMPISTA); // Valkiria

    //ATLETI (Derecha del tablero)

    // Línea frontal (Columna 7)
    equipoAtleti[0] = new Jugador(7, 0, 2, PORTERO);   // Mantícora abajo
    for (int i = 1; i < 8; i++) {
        equipoAtleti[i] = new Jugador(7, i, 2, DELANTERO); // Goblins en el medio
    }
    equipoAtleti[8] = new Jugador(7, 8, 2, PORTERO);   // Mantícora arriba

    // Línea trasera (Columna 8)
    equipoAtleti[9] = new Jugador(8, 0, 2, CENTROCAMPISTA);  // Banshee
    equipoAtleti[10] = new Jugador(8, 1, 2, CENTRAL);        // Troll
    equipoAtleti[11] = new Jugador(8, 2, 2, LATERAL);        // Basilisco
    equipoAtleti[12] = new Jugador(8, 3, 2, MEDIAPUNTA);     // Cambiaformas
    equipoAtleti[13] = new Jugador(8, 4, 2, ENTRENADOR);     // Hechicera (Centro exacto)
    equipoAtleti[14] = new Jugador(8, 5, 2, EXTREMO);        // Dragón
    equipoAtleti[15] = new Jugador(8, 6, 2, LATERAL);        // Basilisco
    equipoAtleti[16] = new Jugador(8, 7, 2, CENTRAL);        // Troll
    equipoAtleti[17] = new Jugador(8, 8, 2, CENTROCAMPISTA); // Banshee
}

void Mundo::dibuja() {
    //RESETEO TOTAL DE OPENGL 
    glDisable(GL_TEXTURE_2D); // Apagamos texturas
    glDisable(GL_LIGHTING);   // Apagamos luces raras
    glDisable(GL_BLEND);      // Apagamos transparencias
    glDisable(GL_DEPTH_TEST); // Evitamos que el fondo se trague al tablero
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Reseteamos el "pincel" a blanco puro
    

   
    miTablero.dibuja();

    // Dibujamos los balones 
    glEnable(GL_TEXTURE_2D);
    for (int i = 0; i < 5; i++) balones[i].dibuja();

    // Apagamos las texturas otra vez para poder dibujar nuestras fichas de colores puros
    glDisable(GL_TEXTURE_2D);
    for (int i = 0; i < 18; i++) {
        if (equipoMadrid[i] != nullptr) equipoMadrid[i]->dibuja();
        if (equipoAtleti[i] != nullptr) equipoAtleti[i]->dibuja();
    }
}

void Mundo::mueve() {
    // De momento vacío, aquí irán las animaciones en el futuro
}

void Mundo::tecla(unsigned char key) {
    // De momento vacío
}

void Mundo::teclaEspecial(int key) {
    // De momento vacío
}