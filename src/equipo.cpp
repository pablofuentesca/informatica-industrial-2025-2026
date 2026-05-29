#include "equipo.h"
#include "jugador.h"

Equipo::Equipo(int _id, const char* _nombre)
    : id(_id), nombre(_nombre), piezas(NUM_PIEZAS, nullptr)
{
}

Equipo::~Equipo()
{
    for (Jugador* pj : piezas)      delete pj;
    for (Jugador* pj : cementerio)  delete pj;
}

void Equipo::coloca(int slot, Jugador* pj)
{
    if (slot >= 0 && slot < (int)piezas.size())
        piezas[slot] = pj;
}

void Equipo::anade(Jugador* pj)
{
    for (Jugador*& slot : piezas)
        if (slot == nullptr) { slot = pj; return; }
    piezas.push_back(pj);
}

Jugador* Equipo::piezaEn(int gx, int gy) const
{
    for (Jugador* pj : piezas)
        if (pj != nullptr && (int)pj->getPosX() == gx && (int)pj->getPosY() == gy)
            return pj;
    return nullptr;
}

Jugador* Equipo::buscaEntrenador() const
{
    for (Jugador* pj : piezas)
        if (pj != nullptr && pj->esEntrenador())
            return pj;
    return nullptr;
}

bool Equipo::tienePiezasVivas() const
{
    for (Jugador* pj : piezas)
        if (pj != nullptr) return true;
    return false;
}

void Equipo::elimina(Jugador* pj)
{
    for (Jugador*& slot : piezas)
        if (slot == pj) { cementerio.push_back(slot); slot = nullptr; return; }
}

void Equipo::curaTodos()
{
    for (Jugador* pj : piezas)
        if (pj != nullptr) pj->curarCompleto();
}

void Equipo::descuentaEncarcelados()
{
    for (Jugador* pj : piezas)
        if (pj != nullptr) pj->descuentaCicloEncarcelado();
}

Jugador* Equipo::ultimaEnCementerio() const
{
    return cementerio.empty() ? nullptr : cementerio.back();
}

void Equipo::sacaDeCementerio()
{
    if (!cementerio.empty()) cementerio.pop_back();
}
