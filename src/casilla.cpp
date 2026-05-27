#include "casilla.h"
#include "freeglut.h"

// Numero de niveles del ciclo de luminiscencia.
// Coincide con el rango admisible de nivelLuz: 0 al 6, con 3 como punto neutro.
static const int NIVEL_NEUTRO = 3;
static const int NIVEL_MAX    = 6;

// Interpolacion lineal entre dos colores RGB.
static void mezclaColor(float t,
                        float r0, float g0, float b0,
                        float r1, float g1, float b1,
                        float& r, float& g, float& b)
{
    r = r0 + t * (r1 - r0);
    g = g0 + t * (g1 - g0);
    b = b0 + t * (b1 - b0);
}

Casilla::Casilla() : tipo(Tipo::LINEA), nivelLuz(NIVEL_NEUTRO) {}

bool Casilla::favoreceLuz() const
{
    if (tipo == Tipo::LINEA) return false;
    // Una casilla CLARA favorece a la luz cuando el ciclo esta hacia claro,
    // una casilla OSCURA favorece a la luz cuando el ciclo va hacia oscuro.
    return (tipo == Tipo::CLARA) ? (nivelLuz > NIVEL_NEUTRO)
                                 : (nivelLuz < NIVEL_NEUTRO);
}

bool Casilla::favoreceOscuro() const
{
    if (tipo == Tipo::LINEA) return false;
    return (tipo == Tipo::CLARA) ? (nivelLuz < NIVEL_NEUTRO)
                                 : (nivelLuz > NIVEL_NEUTRO);
}

void Casilla::dibuja() const
{
    float r = 1.0f, g = 1.0f, b = 1.0f;

    if (tipo == Tipo::LINEA) {
        // Lineas del campo: siempre blancas.
        r = 1.0f; g = 1.0f; b = 1.0f;
    }
    else {
        // t va de -1 (extremo oscuro) a +1 (extremo claro), 0 = neutro.
        float t = (float)(nivelLuz - NIVEL_NEUTRO) / (float)NIVEL_NEUTRO;

        if (tipo == Tipo::CLARA) {
            // Color natural en neutro: verde claro (0.30, 0.80, 0.30)
            // Extremo claro:           verde casi blanco (0.70, 1.00, 0.70)
            // Extremo oscuro:          verde apagado (0.15, 0.45, 0.15)
            if (t >= 0.0f)
                mezclaColor(t,
                            0.30f, 0.80f, 0.30f,
                            0.70f, 1.00f, 0.70f,
                            r, g, b);
            else
                mezclaColor(-t,
                            0.30f, 0.80f, 0.30f,
                            0.15f, 0.45f, 0.15f,
                            r, g, b);
        }
        else {
            // tipo == OSCURA
            // Color natural en neutro: verde oscuro (0.00, 0.20, 0.00)
            // Extremo claro:           verde medio  (0.25, 0.55, 0.25)
            // Extremo oscuro:          casi negro   (0.00, 0.08, 0.00)
            if (t >= 0.0f)
                mezclaColor(t,
                            0.00f, 0.20f, 0.00f,
                            0.25f, 0.55f, 0.25f,
                            r, g, b);
            else
                mezclaColor(-t,
                            0.00f, 0.20f, 0.00f,
                            0.00f, 0.08f, 0.00f,
                            r, g, b);
        }
    }

    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(pos.x,        pos.y);
        glVertex2f(pos.x + 1.0f, pos.y);
        glVertex2f(pos.x + 1.0f, pos.y + 1.0f);
        glVertex2f(pos.x,        pos.y + 1.0f);
    glEnd();
}
