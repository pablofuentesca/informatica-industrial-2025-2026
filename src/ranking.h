#pragma once
#include <string>
#include <vector>

// Una entrada del ranking: el nombre de un bando y cuantas partidas ha ganado.
struct EntradaRanking {
    std::string nombre;
    int         victorias;
};

// Lleva el historico de victorias de cada bando y lo guarda en un fichero de
// texto, de forma que el ranking se conserva entre una ejecucion y la siguiente.
// Responsabilidad unica: gestionar la clasificacion (cargar, guardar, registrar
// una victoria y dibujarse). No conoce las reglas del juego.
class Ranking {
    std::string                 archivo;   // fichero de persistencia
    std::vector<EntradaRanking> entradas;  // ordenadas de mas a menos victorias

    void ordena();   // reordena 'entradas' de mayor a menor numero de victorias

public:
    Ranking(const std::string& nombreArchivo);

    void carga();                                   // lee el ranking del fichero
    void guarda() const;                            // escribe el ranking en el fichero
    void registraVictoria(const std::string& nombre);   // suma 1 victoria y guarda

    int  numEntradas() const { return (int)entradas.size(); }

    void dibuja() const;   // pinta la pantalla de clasificacion (espacio 800x600)
};
