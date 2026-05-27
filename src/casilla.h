#pragma once
#include "posicion.h"

class Casilla {
public:
    // Tipo de la casilla segun el tablero del Archon original:
    //   LINEA  -> casilla fija, no oscila (las lineas blancas del campo)
    //   CLARA  -> casilla del bando luz, oscila ventaja para el bando blanco
    //   OSCURA -> casilla del bando oscuridad, oscila ventaja para el bando rojo
    enum class Tipo { LINEA, CLARA, OSCURA };

private:
    Tipo tipo;
    int  nivelLuz;   // 0 = todo oscuro ... 6 = todo claro; 3 = neutro

public:

    // La posicion en coordenadas de tablero (esquina inferior izquierda).
    Posicion pos;

    Casilla();

    void setTipo(Tipo t)        { tipo = t; }
    void setNivelLuz(int nivel) { nivelLuz = nivel; }

    Tipo getTipo()     const { return tipo; }
    int  getNivelLuz() const { return nivelLuz; }

    // Devuelve true si la casilla esta actualmente del lado "claro" del ciclo.
    // Las lineas son neutras y devuelven false.
    bool favoreceLuz()     const;
    bool favoreceOscuro()  const;

    void dibuja() const;
};
