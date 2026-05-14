// ARCHON_console.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
#include <iostream>
#include "freeglut.h"
#include"tablero.h"
Tablero miTablero;
void OnDraw(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 10.0, -1.0, 10.0); // Esto define que queremos ver desde -1 hasta 10 en X e Y
	glMatrixMode(GL_MODELVIEW);
	//Aqui ira el codigo para dibujar todo lo que se necesite para el partido entre el Real Madrid y el Atleti
	miTablero.dibuja();

    glutSwapBuffers();

}

int main(int argc, char** argv) {
	glutInit(&argc, argv);//Inicializamos la libreria de glut, pasandole los argumentos del main
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//Indicamos el modo de visualizacion, en este caso, doble buffer, RGB y con profundidad
	glutInitWindowSize(800, 800);//El tamaño de la venta sera de 800x800 pixeles
	glutCreateWindow("Real Madrid vs Atletico de Madrid - ARCHON");//Creamos la ventana con el titulo "Real Madrid vs Atletico de Madrid - ARCHON"

	miTablero.inicializa();

	glutDisplayFunc(OnDraw);//La funcion OnDraw sera la encargada de dibujar todo lo que se necesite para el partido
	glutMainLoop();//Iniciamos el bucle principal de glut, que se encargara de llamar a la funcion OnDraw cada vez que sea necesario redibujar la ventana

	

    return 0;
}