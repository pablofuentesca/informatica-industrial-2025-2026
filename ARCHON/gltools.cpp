/* gltools.cpp - implementación de utilidades OpenGL */
#include "gltools.h"
#include "freeglut.h"
#include <iostream>

using namespace std;

std::vector<std::string> GLTools::textures_names;
std::vector<unsigned int> GLTools::textures_ids;

void GLTools::Color(int i, float transparency)
{
	if (i == gltools::BLACK)		glColor4f(0, 0, 0, transparency);
	else if (i == gltools::WHITE)	glColor4f(1.0f, 1.0f, 1.0f, transparency);
	else if (i == gltools::RED)		glColor4f(1.0f, 0, 0, transparency);
	else if (i == gltools::GREEN)	glColor4f(0, 1.0f, 0, transparency);
	else if (i == gltools::BLUE)	glColor4f(0, 0, 1.0f, transparency);
	else if (i == gltools::MAGENTA)	glColor4f(1.0f, 0, 1.0f, transparency);
	else if (i == gltools::CYAN)	glColor4f(0.0f, 1.0f, 1.0f, transparency);
	else if (i == gltools::GREY)	glColor4f(0.5f, 0.5f, 0.5f, transparency);
	else if (i == gltools::YELLOW)  glColor4f(1.0f, 1.0f, 0.0f, transparency);
	else if (i == gltools::ORANGE)  glColor4f(1.0f, 0.5f, 0.0f, transparency);
	else if (i == gltools::PURPLE)  glColor4f(0.3, 0.3, 1.0f, transparency);
	else if (i == gltools::PINK) 	glColor4f(1.0f, 0.3f, 0.3f, transparency);
}

void GLTools::BackgroundColor(int i)
{
	if (i == gltools::BLACK)		glClearColor(0, 0, 0, 0);
	else if (i == gltools::WHITE)	glClearColor(1.0f, 1.0f, 1.0f, 0);
	else if (i == gltools::RED)		glClearColor(1.0f, 0, 0, 0);
	else if (i == gltools::GREEN)	glClearColor(0, 1.0f, 0, 0);
	else if (i == gltools::BLUE)	glClearColor(0, 0, 1.0f, 0);
}

void GLTools::Print(const std::string& msg, float x, float y, float z, int fuente) {
	glDisable(GL_LIGHTING);
	int len = (int)msg.size();
	glRasterPos3f(x, y, z);

	switch (fuente) {
	case 1:
		for (int i = 0; i < len; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, msg[i]);
		break;
	case 2:
		for (int i = 0; i < len; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, msg[i]);
		break;
	case 3:
		for (int i = 0; i < len; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg[i]);
		break;
	default:
		for (int i = 0; i < len; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg[i]);
	}
	glEnable(GL_LIGHTING);
}

unsigned int GLTools::LoadTexture(const std::string& nombre)
{
	for (unsigned int i = 0; i < textures_ids.size(); i++)
	{
		if (textures_names[i] == nombre)
			return textures_ids[i];
	}

	unsigned int textura = 0;
	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Implementar carga real si se necesita
	textures_ids.push_back(textura);
	textures_names.push_back(nombre);
	return textura;
}

void GLTools::DeleteTextures()
{
	for (unsigned int i = 0; i < textures_ids.size(); i++)
		glDeleteTextures(1, &textures_ids[i]);
	textures_ids.clear();
	textures_names.clear();
	glBindTexture(GL_TEXTURE_2D, 0);
}

// (las funciones DrawFrame/DrawGrid/DrawExactGrid pueden permanecer como estaban)