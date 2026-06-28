#pragma once
#include <string>
// Gestiona la entrada de texto por teclado
class CampoTexto {
    std::string texto;        // lo escrito hasta ahora
    int maxLongitud;  // numero maximo de caracteres

public:
    CampoTexto(int maximo = 12);
    void anadeCaracter(char c);   // añade un caracter
    void borra();                 // borra el ultimo caracter
    void limpia();                // vacia el campo
    bool vacio() const { return texto.empty(); }
    const std::string& valor() const { return texto; }
    void dibuja(float x, float y) const;   // pinta la caja, el texto y el cursor
};
