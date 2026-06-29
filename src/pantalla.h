#pragma once
// Cada pantalla concreta hereda de esta 
class Pantalla {
public:
    virtual ~Pantalla() {}

    virtual void inicializa()= 0;
    virtual void dibuja() const = 0;
    virtual void mueve(double dt)          {}
    virtual void tecla(unsigned char key)  {}
    virtual void teclaArriba(unsigned char key) {}
    virtual void teclaEspecial(int key)    {}
    virtual void teclaEspecialArriba(int key) {}
    virtual void raton(int boton, int estado, int x, int y) {}
};
