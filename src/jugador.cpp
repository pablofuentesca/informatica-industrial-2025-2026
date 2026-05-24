#include "jugador.h"
#include "ETSIDI.h"
#include "freeglut.h"
#include <string>
#include <iostream>

Jugador::Jugador(float x, float y, int _equipo, RolFutbol _rol) {
    radio = 0.4f;
    pos.x = x + 0.5f;
    pos.y = y + 0.5f;
    equipo = _equipo;
    rol = _rol;
    sprite = nullptr;

    rangoMovimiento = 0;
    esVolador = false;

    //Configuramos las stats
    switch (rol) {
    case PORTERO:        rangoMovimiento = 3; esVolador = false; break;
    case DELANTERO:      rangoMovimiento = 3; esVolador = false; break;
    case CENTRAL:        rangoMovimiento = 3; esVolador = false; break;
    case LATERAL:        rangoMovimiento = 4; esVolador = false; break;
    case CENTROCAMPISTA: rangoMovimiento = 3; esVolador = true;  break;
    case MEDIAPUNTA:     rangoMovimiento = 4; esVolador = true;  break;
    case EXTREMO:        rangoMovimiento = 5; esVolador = true;  break;
    case ENTRENADOR:     rangoMovimiento = 3; esVolador = false; break;
    }

    //Carga de Imágenes
    std::string rutaBase = "bin/imagenes/fotosjugadores/";
    std::string prefix = (equipo == 1) ? "madrid" : "atleti";
    std::string suffix = "";

    switch (rol) {
    case PORTERO:        suffix = "portero.png"; break;
    case CENTRAL:        suffix = "defensa.png"; break;
    case LATERAL:        suffix = "lateral.png"; break;
    case CENTROCAMPISTA: suffix = "centro.png"; break;
    case MEDIAPUNTA:     suffix = "mediapunta.png"; break;
    case EXTREMO:        suffix = "extremo.png"; break;
    case ENTRENADOR:     suffix = "entrenador.png"; break;
    case DELANTERO:      suffix = "delantero.png"; break; 
    default:             suffix = "portero.png"; break;   //EL SALVAVIDAS ANTI-CRASHEOS!
    }

    // Usamos ruta doble (con y sin ../) para que lo encuentre se arranque como se arranque
    std::string rutaOriginal = "../" + rutaBase + prefix + suffix;
    std::string rutaDirecta = rutaBase + prefix + suffix;

    try {
        sprite = new ETSIDI::Sprite(rutaOriginal.c_str(), pos.x, pos.y, radio * 2, radio * 2);
    }
    catch (...) {
        try {
            sprite = new ETSIDI::Sprite(rutaDirecta.c_str(), pos.x, pos.y, radio * 2, radio * 2);
        }
        catch (...) {
            std::cout << "Fallo al cargar: " << prefix << suffix << std::endl;
        }
    }
}

Jugador::~Jugador() {
    if (sprite != nullptr) delete sprite;
}

void Jugador::dibuja() const {
    
    if (sprite != nullptr) {
        // Encendemos texturas solo para la imagen
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Permite transparencias del PNG

        sprite->draw();

        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    }
}

void Jugador::mover(float dirX, float dirY) {
    float nuevaX = pos.x + dirX;
    float nuevaY = pos.y + dirY;

    if (nuevaX > 0.0f && nuevaX < 9.0f && nuevaY > 0.0f && nuevaY < 9.0f) {
        pos.x = nuevaX;
        pos.y = nuevaY;
        if (sprite != nullptr) sprite->setPos(pos.x, pos.y);
    }
}