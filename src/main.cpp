// ARCHON_console.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
#include <iostream>
#include "freeglut.h"

void OnDraw(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Aqui ira el codigo para dibujar todo lo que se necesite para el partido entre el Real Madrid y el Atleti
    glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);//Inicializamos la libreria de glut, pasandole los argumentos del main
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//Indicamos el modo de visualizacion, en este caso, doble buffer, RGB y con profundidad
	glutInitWindowSize(800, 800);//El tamaño de la venta sera de 800x800 pixeles
	glutCreateWindow("Real Madrid vs Atletico de Madrid - ARCHON");//Creamos la ventana con el titulo "Real Madrid vs Atletico de Madrid - ARCHON"

	glutDisplayFunc(OnDraw);//La funcion OnDraw sera la encargada de dibujar todo lo que se necesite para el partido
	glutMainLoop();//Iniciamos el bucle principal de glut, que se encargara de llamar a la funcion OnDraw cada vez que sea necesario redibujar la ventana

    return 0;
}