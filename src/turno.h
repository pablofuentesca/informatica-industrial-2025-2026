#pragma once

// Controla a quien le toca jugar y si todavia es la primera jugada de la partida.
// Encapsula lo que antes eran dos variables sueltas en Mundo (turnoEquipo, primerTurno).
class Turno {
    int  equipoActual;   // 1 = Madrid/Blanco, 2 = Atleti/Rojo
    bool primero;        // true hasta que se realiza la primera jugada

public:
    Turno();

    void sortea();   // tirada de moneda inicial: decide quien saca

    int  equipoEnJuego() const { return equipoActual; }
    bool esPrimerTurno() const { return primero; }

    // pasa el turno al otro equipo y marca que ya no es la primera jugada
    void cambia();
};
