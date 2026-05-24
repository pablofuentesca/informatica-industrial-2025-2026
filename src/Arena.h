#pragma once

class Arena {
    double xMin{ 50 }, xMax{ 750 }, yMin{ 50 }, yMax{ 550 };
    double tam{ 25 };          // Mitad del lado del rectángulo

    double j1x{ 200 }, j1y{ 300 };   // Jugador 1 - Madrid (blanco)
    double j2x{ 600 }, j2y{ 300 };   // Jugador 2 - Atleti  (rojo)

public:
    void dibuja() const;
    void tecla(unsigned char key);
    void teclaEspecial(int key);
};
