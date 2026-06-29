#pragma once
// Clase base de todas las pantallas
struct Contexto;

class Pantalla {
protected:
    Contexto* ctx{ nullptr };  
public:
    virtual ~Pantalla() {}
    void setContexto(Contexto* c) { ctx = c; }

    virtual void inicializa() {}                
    virtual void dibuja() const = 0;             
    virtual void mueve(double dt) {}
    virtual void tecla(unsigned char key) {}
    virtual void teclaArriba(unsigned char key) {}
    virtual void teclaEspecial(int key) {}
    virtual void teclaEspecialArriba(int key) {}
    virtual void raton(int boton, int estado, float x, float y) {}
};
