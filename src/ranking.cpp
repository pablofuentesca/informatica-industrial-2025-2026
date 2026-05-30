#include "ranking.h"
#include "freeglut.h"
#include <fstream>

// Helper local para escribir un texto en la pantalla en la posicion (x, y).
static void texto(float x, float y, const char* cadena, float r, float g, float b)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (const char* c = cadena; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

Ranking::Ranking(const std::string& nombreArchivo) : archivo(nombreArchivo)
{
    carga();
}

void Ranking::carga()
{
    entradas.clear();

    std::ifstream fichero(archivo);
    if (!fichero.is_open()) return;   // la primera vez aun no existe el fichero

    // Cada linea tiene el formato:  NOMBRE;VICTORIAS
    // Leemos la linea entera (el nombre puede tener espacios) y la partimos
    // por el caracter ';'.
    std::string linea;
    while (std::getline(fichero, linea)) {
        if (linea.empty()) continue;

        std::string::size_type sep = linea.find(';');
        if (sep == std::string::npos) continue;

        EntradaRanking e;
        e.nombre    = linea.substr(0, sep);
        e.victorias = std::stoi(linea.substr(sep + 1));
        entradas.push_back(e);
    }

    ordena();
}

void Ranking::guarda() const
{
    std::ofstream fichero(archivo);
    if (!fichero.is_open()) return;

    for (const EntradaRanking& e : entradas)
        fichero << e.nombre << ';' << e.victorias << '\n';
}

void Ranking::registraVictoria(const std::string& nombre)
{
    // Si el bando ya esta en el ranking le sumamos una victoria;
    // si no, lo agregamos como entrada nueva con una victoria.
    for (EntradaRanking& e : entradas)
        if (e.nombre == nombre) {
            e.victorias++;
            ordena();
            guarda();
            return;
        }

    EntradaRanking nueva{ nombre, 1 };
    entradas.push_back(nueva);
    ordena();
    guarda();
}

void Ranking::ordena()
{
    // Ordenacion por burbuja: deja las entradas de mayor a menor victorias.
    for (int i = 0; i < (int)entradas.size(); i++)
        for (int j = i + 1; j < (int)entradas.size(); j++)
            if (entradas[j].victorias > entradas[i].victorias) {
                EntradaRanking aux = entradas[i];
                entradas[i]        = entradas[j];
                entradas[j]        = aux;
            }
}

void Ranking::dibuja() const
{
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    // Fondo oscuro con borde dorado (mismo estilo que la pantalla de reglas).
    glColor3f(0.03f, 0.06f, 0.03f);
    glBegin(GL_QUADS);
        glVertex2f(30, 30); glVertex2f(770, 30);
        glVertex2f(770, 570); glVertex2f(30, 570);
    glEnd();
    glColor3f(0.7f, 0.55f, 0.05f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(30, 30); glVertex2f(770, 30);
        glVertex2f(770, 570); glVertex2f(30, 570);
    glEnd();
    glLineWidth(1.0f);

    // Titulo y cabeceras.
    texto(320, 535, "== RANKING ==", 0.9f, 0.75f, 0.1f);
    texto(110, 495, "POS",       1.0f, 1.0f, 1.0f);
    texto(210, 495, "EQUIPO",    1.0f, 1.0f, 1.0f);
    texto(560, 495, "VICTORIAS", 1.0f, 1.0f, 1.0f);

    if (entradas.empty()) {
        texto(230, 430, "Aun no se ha jugado ninguna partida", 0.7f, 0.7f, 0.7f);
        texto(300, 60, "Pulsa ESC para volver", 0.55f, 0.55f, 0.55f);
        return;
    }

    // Una fila por entrada, ya ordenadas de mas a menos victorias.
    float y   = 460;
    int   pos = 1;
    for (const EntradaRanking& e : entradas) {
        std::string sPos = std::to_string(pos);
        std::string sVic = std::to_string(e.victorias);

        texto(120, y, sPos.c_str(),        0.85f, 0.85f, 1.0f);
        texto(210, y, e.nombre.c_str(),    0.9f,  0.9f,  0.9f);
        texto(590, y, sVic.c_str(),        0.9f,  0.85f, 0.3f);

        y -= 35.0f;
        pos++;
    }

    texto(300, 60, "Pulsa ESC para volver", 0.55f, 0.55f, 0.55f);
}
