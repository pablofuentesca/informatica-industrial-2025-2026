// ARCHON_console.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
#include <iostream>
#include "freeglut.h"
#include"tablero.h"
#include "pelota.h"
#include "centrocampista.h"

// Creamos un jugador de cada equipo. 
// Parámetros: (Posición X, Posición Y, Equipo)
Centrocampista jugadorMadrid(1, 4, 1); // Equipo 1 (Luz/Madrid)
Centrocampista jugadorAtleti(7, 4, 2); // Equipo 2 (Oscuridad/Atleti)

Tablero miTablero;
Pelota balones[5] = {
	Pelota(4, 4), // Balón del centro
	Pelota(0, 4), // Balón extremo izquierdo
	Pelota(8, 4), // Balón extremo derecho
	Pelota(4, 0), // Balón extremo inferior
	Pelota(4, 8)  // Balón extremo superior
};

void OnDraw(void) {
	// 1. Limpiamos la pantalla
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 2. Reseteamos la cámara
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 10.0, -1.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// ---------------------------------------------------
	// 3. EL ANTÍDOTO: Apagamos todo lo que ETSIDI deja encendido
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	// ---------------------------------------------------

	// 4. Dibujamos el césped limpio
	miTablero.dibuja();

	// 5. Dibujamos los sprites (ETSIDI volverá a encender sus cosas automáticamente)
	for (int i = 0; i < 5; i++) balones[i].dibuja();
	jugadorMadrid.dibuja();
	jugadorAtleti.dibuja();

	glutSwapBuffers();

}
// Función para las teclas normales (WASD para el Madrid)
void OnKeyboardDown(unsigned char key, int x_t, int y_t) {
	if (key == 'w' || key == 'W') jugadorMadrid.mover(0.0f, 1.0f);  // Arriba
	if (key == 's' || key == 'S') jugadorMadrid.mover(0.0f, -1.0f); // Abajo
	if (key == 'a' || key == 'A') jugadorMadrid.mover(-1.0f, 0.0f); // Izquierda
	if (key == 'd' || key == 'D') jugadorMadrid.mover(1.0f, 0.0f);  // Derecha

	// Obligamos a la pantalla a refrescarse para ver el cambio
	glutPostRedisplay();
}

// Función para las flechas (Para el Atleti)
void OnSpecialKeyboardDown(int key, int x, int y) {
	if (key == GLUT_KEY_UP) jugadorAtleti.mover(0.0f, 1.0f);
	if (key == GLUT_KEY_DOWN) jugadorAtleti.mover(0.0f, -1.0f);
	if (key == GLUT_KEY_LEFT) jugadorAtleti.mover(-1.0f, 0.0f);
	if (key == GLUT_KEY_RIGHT) jugadorAtleti.mover(1.0f, 0.0f);

	glutPostRedisplay();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);//Inicializamos la libreria de glut, pasandole los argumentos del main
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//Indicamos el modo de visualizacion, en este caso, doble buffer, RGB y con profundidad
	glutInitWindowSize(800, 800);//El tamaño de la venta sera de 800x800 pixeles
	glutCreateWindow("Real Madrid vs Atletico de Madrid - ARCHON");//Creamos la ventana con el titulo "Real Madrid vs Atletico de Madrid - ARCHON"

	miTablero.inicializa();

	glutDisplayFunc(OnDraw);//La funcion OnDraw sera la encargada de dibujar todo lo que se necesite para el partido
	
	
	glutKeyboardFunc(OnKeyboardDown);//Indicamos que la funcion OnKeyboardDown se encargara de gestionar las teclas normales (WASD para el Madrid)
	glutSpecialFunc(OnSpecialKeyboardDown);//Indicamos que la funcion OnSpecialKeyboardDown se encargara de gestionar las teclas especiales (Flechas para el Atleti)

	glutMainLoop();//Iniciamos el bucle principal de glut, que se encargara de llamar a la funcion OnDraw cada vez que sea necesario redibujar la ventana

	

    return 0;
}

