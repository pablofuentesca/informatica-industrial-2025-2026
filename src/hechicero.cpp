#include "hechicero.h"

Hechicero::Hechicero()
{
    conjuros.push_back(new ConjuroTeleport());
    conjuros.push_back(new ConjuroCurar());
    conjuros.push_back(new ConjuroCambioCiclo());
    conjuros.push_back(new ConjuroIntercambiar());
    conjuros.push_back(new ConjuroInvocar());
    conjuros.push_back(new ConjuroRevivir());
    conjuros.push_back(new ConjuroEncarcelar());
}

Hechicero::~Hechicero()
{
    for (Conjuro* c : conjuros) delete c;
}

const char* Hechicero::nombreConjuro(int i) const
{
    if (i < 0 || i >= (int)conjuros.size()) return "";
    return conjuros[i]->nombre();
}

bool Hechicero::conjuroUsado(int i) const
{
    if (i < 0 || i >= (int)conjuros.size()) return true;
    return conjuros[i]->estaUsado();
}

void Hechicero::marcarConjuroUsado(int i)
{
    if (i >= 0 && i < (int)conjuros.size()) conjuros[i]->marcarUsado();
}

bool Hechicero::lanzarConjuro(int i, Mundo& m, int equipo)
{
    if (!tieneConjuros()) return false;
    if (i < 0 || i >= (int)conjuros.size()) return false;
    if (conjuros[i]->estaUsado()) return false;

    if (conjuros[i]->aplicar(m, equipo)) {
        gastaConjuro();
        return true;
    }
    return false;
}
