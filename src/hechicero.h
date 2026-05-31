#pragma once
#include "conjuro.h"
#include <vector>
class Mundo;

class Hechicero {
    int conjurosRestantes{ 7 };
    std::vector<Conjuro*> conjuros;

public:
    Hechicero();
    ~Hechicero();
    Hechicero(const Hechicero&) = delete;
    Hechicero& operator=(const Hechicero&) = delete;

    int  getConjurosRestantes() const { return conjurosRestantes; }
    bool tieneConjuros()        const { return conjurosRestantes > 0; }

    bool gastaConjuro() {
        if (conjurosRestantes <= 0) return false;
        conjurosRestantes--;
        return true;
    }

    int         numConjuros()        const { return (int)conjuros.size(); }
    const char* nombreConjuro(int i) const;
    bool        conjuroUsado(int i)  const;
    void        marcarConjuroUsado(int i);

    bool lanzarConjuro(int i, Mundo& m, int equipo);
};
