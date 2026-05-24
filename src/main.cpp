#include <iostream>
#include "freeglut.h"
#include "coordinador.h"

//única variable global de todo el proyectO
Coordinador coordinador;

void OnDraw(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // El coordinador decide soberanamente qué dibujar
    coordinador.dibuja();

    glutSwapBuffers();
}

void OnKeyboardDown(unsigned char key, int x_t, int y_t) {
    coordinador.tecla(key);
    glutPostRedisplay();
}

void OnSpecialKeyboardDown(int key, int x, int y) {
    coordinador.teclaEspecial(key);
    glutPostRedisplay();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Real Madrid vs Atletico de Madrid - ARCHON");

    // Ya NO llamamos a inicializarEquipos ni mundos aquí. El coordinador lo gestionará.

    glutDisplayFunc(OnDraw);
    glutKeyboardFunc(OnKeyboardDown);
    glutSpecialFunc(OnSpecialKeyboardDown);

    coordinador.inicializa();
    glutMainLoop();
    return 0;
}

