#pragma once

namespace ETSIDI { class Sprite; }

// Vista de una pieza en el tablero.
// Separa el renderizado (ETSIDI::Sprite) de la logica (Jugador).
// Jugador ya no conoce ETSIDI; VistaJugador crea y destruye el sprite.
class VistaJugador {
    ETSIDI::Sprite* sprite;

public:
    VistaJugador(const char* rutaTextura, float posX, float posY, float radio);
    ~VistaJugador();
    VistaJugador(const VistaJugador&) = delete;
    VistaJugador& operator=(const VistaJugador&) = delete;

    // Dibuja la pieza en la posicion actual que le pasa Mundo
    void dibuja(float posX, float posY) const;
};
