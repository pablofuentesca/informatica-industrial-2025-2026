#include <chrono>
#include "freeglut.h"
#include "coordinador.h"

using namespace std::chrono;


Coordinador coordinador;

void OnDraw(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    coordinador.dibuja();
    glutSwapBuffers();
}

void OnTimer(int valor)
{
    static high_resolution_clock::time_point tAnterior = high_resolution_clock::now();
    high_resolution_clock::time_point tAhora = high_resolution_clock::now();
    double dt = duration<double>(tAhora - tAnterior).count();
    tAnterior = tAhora;
    if (dt > 0.05) dt = 0.05;  // limite por si la ventana se congela
    coordinador.mueve(dt);
    glutPostRedisplay();
    glutTimerFunc(16, OnTimer, 0);
}

void OnMouseClick(int button, int state, int x, int y) {
    coordinador.raton(button, state, x, y);
    glutPostRedisplay(); // Para que dibuje el cambio al instante
}

void OnKeyboardDown(unsigned char key, int x_t, int y_t) {
    coordinador.tecla(key);
    glutPostRedisplay();
}

void OnKeyboardUp(unsigned char key, int x, int y) {
    coordinador.teclaJ1(key);
}

void OnSpecialKeyboardDown(int key, int x, int y) {
    coordinador.teclaEspecial(key);
    glutPostRedisplay();
}

void OnSpecialKeyboardUp(int key, int x, int y) {
    coordinador.teclaEspecialArriba(key);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Real Madrid vs Atletico de Madrid - ARCHON");

    glutDisplayFunc(OnDraw);
    glutTimerFunc(16, OnTimer, 0);
    glutKeyboardFunc(OnKeyboardDown);
    glutKeyboardUpFunc(OnKeyboardUp);
    glutSpecialFunc(OnSpecialKeyboardDown);
    glutSpecialUpFunc(OnSpecialKeyboardUp);
    glutMouseFunc(OnMouseClick);

    coordinador.inicializa();
    glutMainLoop();
    return 0;
}
