// game.cpp: entry point for a general outline of boardgame using glut and OpenGL

#include <iostream>
#include "boardgl.h"
#include "include/glut.h"

#define GRID_SIZE	5		//NxN grid

// callback declarations
void OnDraw(void);
void OnKeyboardDown(unsigned char key, int x, int y);
void OnMouseClick(int button, int state, int x, int y);

// global objects
Board gameboard(GRID_SIZE);
BoardGL scene(&gameboard);

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("MiJuego");

	glutDisplayFunc(OnDraw);
	glutKeyboardFunc(OnKeyboardDown);
	glutMouseFunc(OnMouseClick);

	scene.init();
	glutMainLoop();
	return 0;
}

void OnDraw(void) {
	scene.Draw();
	glutSwapBuffers();
}

void OnKeyboardDown(unsigned char key, int x_t, int y_t) {
	scene.KeyDown(key);
	glutPostRedisplay();
}

void OnMouseClick(int b, int state, int x, int y) {
	bool down = (state == GLUT_DOWN);
	int button = 0;
	if (b == GLUT_LEFT_BUTTON) {
		button = MOUSE_LEFT_BUTTON;
	}
	else if (b == GLUT_RIGHT_BUTTON) {
		button = MOUSE_RIGHT_BUTTON;
		std::cout << "MOUSE_RIGHT_BUTTON" << std::endl;
	}

	int specialKey = glutGetModifiers();
	bool ctrlKey = (specialKey & GLUT_ACTIVE_CTRL) ? true : false;
	bool sKey = (specialKey & GLUT_ACTIVE_SHIFT) ? true : false;

	// pasar el botón mapeado (button), no el valor GLUT original
	scene.MouseButton(x, y, button, down, sKey, ctrlKey);
	glutPostRedisplay();
}
