#include "campotexto.h"
#include "freeglut.h"

CampoTexto::CampoTexto(int maximo) : maxLongitud(maximo){}

void CampoTexto::anadeCaracter(char c)
{
    // solo caracteres imprimibles y mientras no se pase del maximo
    if (c >= 32 && c < 127 && (int)texto.size() < maxLongitud)
        texto += c;
}

void CampoTexto::borra()
{
    if (!texto.empty()) texto.pop_back();
}

void CampoTexto::limpia()
{
    texto.clear();
}

void CampoTexto::dibuja(float x, float y) const
{
    float ancho = 300.0f;
    float alto = 45.0f;

    // fondo negro de la caja
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y); glVertex2f(x + ancho, y);
    glVertex2f(x + ancho, y + alto); glVertex2f(x, y + alto);
    glEnd();

    // borde gris
    glColor3f(0.50f, 0.50f, 0.55f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y); glVertex2f(x + ancho, y);
    glVertex2f(x + ancho, y + alto); glVertex2f(x, y + alto);
    glEnd();

    // el texto con un "_" al final que hace de cursor
    std::string linea = texto + "_";
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x + 15.0f, y + 15.0f);
    for (const char* c = linea.c_str(); *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}
