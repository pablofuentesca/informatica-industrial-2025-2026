#pragma once
#include <vector>

class Jugador;

// Agrupa todas las piezas de un bando y su cementerio.
// Antes eran 4 vectores sueltos en Mundo (equipoMadrid/Atleti, cementerioMadrid/Atleti);
// esta clase los encapsula junto con las operaciones que se hacian sobre ellos.
class Equipo {
public:
    static constexpr int NUM_PIEZAS = 18;   // plantilla inicial fija de cada bando

private:
    int         id;          // 1 = Madrid, 2 = Atleti
    const char* nombre;      // "REAL MADRID" / "ATLETICO"
    std::vector<Jugador*> piezas;      // NUM_PIEZAS slots; nullptr = pieza muerta/vacia
    std::vector<Jugador*> cementerio;  // piezas eliminadas (para revivir)

public:
    Equipo(int id, const char* nombre);
    ~Equipo();
    Equipo(const Equipo&) = delete;//contructor de copia
    Equipo& operator=(const Equipo&) = delete;

    int         getId()     const { return id; }
    const char* getNombre() const { return nombre; }

    // construccion del equipo
    void coloca(int slot, Jugador* pj);   // sustituye a  equipoX[slot] = new ...
    void anade(Jugador* pj);              // reutiliza un slot libre o crece (invocar/revivir)

    
    Jugador* piezaEn(int gx, int gy) const;   // pieza en esa casilla, o nullptr
    Jugador* buscaEntrenador()       const;   // entrenador vivo, o nullptr
    bool     tienePiezasVivas()      const;

    // operaciones de combate / conjuros
    void elimina(Jugador* pj);                // pasa la pieza al cementerio
    bool curaUna();              // cura del todo la primera pieza herida; false si ninguna lo esta
    void descuentaEncarcelados();
    void descuentaTemporales();  // retira los elementales cuya vida temporal ha expirado

    // gestion del cementerio, para el conjuro Revivir
    Jugador* ultimaEnCementerio() const;      // peek; nullptr si esta vacio
    void     sacaDeCementerio();              // pop de la ultima

    // recorrido, para dibujar / buscar
    const std::vector<Jugador*>& getPiezas() const { return piezas; }
};
