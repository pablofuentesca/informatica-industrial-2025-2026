#ifndef _GLTOOLS_H
#define _GLTOOLS_H

#ifdef _WIN32
#include <windows.h>
#endif

// Usamos la cabecera GLUT que existe en el repo: include/glut.h
#include "include/glut.h"
#include "freeglut.h"
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>

namespace gltools {
	enum { WHITE, RED, GREEN, BLUE, MAGENTA, CYAN, YELLOW, ORANGE, PURPLE, PINK, GREY, BLACK };
}

class GLTools
{
public:
	// Color utilities
	static void Color(int color, float transparency = 1.0f);
	static void BackgroundColor(int color);
	// Textures utilities
	static unsigned int LoadTexture(const std::string& filename);
	static void DeleteTextures();
	// Text utilities
	static void Print(const std::string& msg, float x = 0, float y = 0, float z = 0, int fuente = 1);

	// Primitives
	static void DrawEllipse(float xo, float yo, float tho, float A, float B, float C, float scale = 1);
	static void DrawFrame();
	static void DrawGrid(int num_cells, float size);
	static void DrawGrid(int num_cells, float size, int col, float transparency = 1.0f);
	static void DrawExactGrid(int N, float size, int col, float transparency = 1.0f);

protected:
	static std::vector<std::string> textures_names;
	static std::vector<unsigned int> textures_ids;
};

#endif