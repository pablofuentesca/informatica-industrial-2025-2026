#include "tablero.h"

Tablero::Tablero() : cicloLuz(3), direccion(1)
{
    inicializa();
}

void Tablero::inicializa()
{
    // Mapa exacto del tablero de la imagen de Archon
    //   'W' = linea blanca del campo (no oscila)
    //   'B' = casilla del bando OSCURO  (verde oscuro en neutro)
    //   'G' = casilla del bando CLARO   (verde claro  en neutro)
    char mapa[9][9] = {
        {'B', 'G', 'B', 'W', 'W', 'W', 'G', 'B', 'G'}, // Fila 0 (Abajo)
        {'G', 'B', 'W', 'G', 'W', 'B', 'W', 'G', 'B'}, // Fila 1
        {'B', 'W', 'G', 'B', 'W', 'G', 'B', 'W', 'G'}, // Fila 2
        {'W', 'G', 'B', 'G', 'W', 'B', 'G', 'B', 'W'}, // Fila 3
        {'G', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'B'}, // Fila 4 (Centro)
        {'W', 'G', 'B', 'G', 'W', 'B', 'G', 'B', 'W'}, // Fila 5
        {'B', 'W', 'G', 'B', 'W', 'G', 'B', 'W', 'G'}, // Fila 6
        {'G', 'B', 'W', 'G', 'W', 'B', 'W', 'G', 'B'}, // Fila 7
        {'B', 'G', 'B', 'W', 'W', 'W', 'G', 'B', 'G'}  // Fila 8 (Arriba)
    };

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            casillas[i][j].pos.x = (float)i;
            casillas[i][j].pos.y = (float)j;

            char color = mapa[j][i];
            if      (color == 'W') casillas[i][j].setTipo(Casilla::Tipo::LINEA);
            else if (color == 'G') casillas[i][j].setTipo(Casilla::Tipo::CLARA);
            else                   casillas[i][j].setTipo(Casilla::Tipo::OSCURA);
        }
    }

    cicloLuz  = 3;
    direccion = 1;
    propagaCicloACasillas();
}

void Tablero::dibuja() const
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            casillas[i][j].dibuja();
}

void Tablero::avanzarCiclo()
{
    cicloLuz += direccion;
    // Rebote en los extremos: el ciclo va de 0 a 6 y vuelve.
    if (cicloLuz >= 6) { cicloLuz = 6; direccion = -1; }
    if (cicloLuz <= 0) { cicloLuz = 0; direccion = +1; }
    propagaCicloACasillas();
}

void Tablero::propagaCicloACasillas()
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            casillas[i][j].setNivelLuz(cicloLuz);
}

bool Tablero::casillaFavoreceLuz(int gx, int gy) const
{
    if (gx < 0 || gx >= 9 || gy < 0 || gy >= 9) return false;
    return casillas[gx][gy].favoreceLuz();
}

bool Tablero::casillaFavoreceOscuro(int gx, int gy) const
{
    if (gx < 0 || gx >= 9 || gy < 0 || gy >= 9) return false;
    return casillas[gx][gy].favoreceOscuro();
}
