#include "jugador.h"
#include <algorithm>

Jugador::Jugador(float x, float y, int _equipo, const char* ruta)
{
    pos.x = x + 0.5f;
    pos.y = y + 0.5f;
    equipo = _equipo;
    rutaTextura = ruta;
}

Jugador::~Jugador() {}
void Jugador::moverA(float x, float y)
{
    pos.x = x + 0.5f;
    pos.y = y + 0.5f;
}

void Jugador::mover(float dirX, float dirY)
{
    float nuevaX = pos.x + dirX;
    float nuevaY = pos.y + dirY;

    if (nuevaX > 0.0f && nuevaX < 9.0f && nuevaY > 0.0f && nuevaY < 9.0f) 
    {
        pos.x = nuevaX;
        pos.y = nuevaY;
    }
}

float Jugador::getPosX() const
{
    return pos.x;
}

float Jugador::getPosY() const
{
    return pos.y;
}

void Jugador::setPosicion(float nuevaX, float nuevaY)
{
    pos.x = nuevaX;
    pos.y = nuevaY;
}

bool Jugador::esMovimientoValido(int origenX, int origenY, int destinoX, int destinoY) const
{
    if (origenX == destinoX && origenY == destinoY) return false;

    int difX = std::abs(destinoX - origenX);
    int difY = std::abs(destinoY - origenY);
    int distancia = (std::max)(difX, difY);

    return (distancia <= getRango());
}

void Jugador::actualiza(double dt)
{
	// actualiza cooldown de ataque
    if (timerAtaque > 0.0) timerAtaque -= dt;
    if (timerAtaque < 0.0) timerAtaque = 0.0;
}

void Jugador::recibeGolpe(int dano)
{
    hp -= dano;
    if (hp < 0) hp = 0;
}

double Jugador::alcanceAtaque() const
{
    return 0.0;   
}

void Jugador::habilidadEspecial()
{
    
}
