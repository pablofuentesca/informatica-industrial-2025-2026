#include "Arena.h"
#include "jugador.h"
#include "interaccionArena.h"
#include "ETSIDI.h"
#include "freeglut.h"
#include <cmath>
#include <cstring>

static const double PI = 3.14159265358979;

static double limitacionRangoX(double x) {
    if (x < 0.0) return 0.0;
    if (x > 1.0) return 1.0;
    return x;
}

static double smoothstep(double x) {
    return x * x * (3.0 - 2.0 * x);
}

static void dibujaCirculo(double cx, double cy, double r, int seg = 64)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < seg; i++) {
        double a = 2.0 * PI * i / seg;
        glVertex2d(cx + r * cos(a), cy + r * sin(a));
    }
    glEnd();
}

// banda diagonal negra que barre la pantalla de izquierda a derecha
static void dibujaDiagonal(double prog)
{
    double xBajo   = (1.0 - prog) * 1400.0;
    double xArriba = xBajo - 600.0;
    glColor3d(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
        glVertex2d(-200.0, 0.0);
        glVertex2d(xBajo,  0.0);
        glVertex2d(xArriba, 600.0);
        glVertex2d(-200.0, 600.0);
    glEnd();
}

// iris que se abre desde el centro
static void dibujaIris(double prog)
{
    double cx  = 400.0, cy = 300.0;
    double rExt = 520.0;
    double rInt = prog * rExt;
    glColor3d(0.0, 0.0, 0.0);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= 64; i++) {
        double a = 2.0 * PI * i / 64;
        glVertex2d(cx + rExt * cos(a), cy + rExt * sin(a));
        glVertex2d(cx + rInt * cos(a), cy + rInt * sin(a));
    }
    glEnd();
}

// nombre real del jugador de futbol segun la ruta de textura
static const char* nombreJugador(const char* ruta)
{
    if (!ruta || !ruta[0]) return "?";
    if (strstr(ruta, "portero"))        return strstr(ruta, "madrid") ? "Courtois"   : "Morata";
    if (strstr(ruta, "delantero"))      return strstr(ruta, "madrid") ? "Mbappe"     : "Delantero";
    if (strstr(ruta, "defensa"))        return strstr(ruta, "madrid") ? "Militao"    : "Savic";
    if (strstr(ruta, "lateral"))        return strstr(ruta, "madrid") ? "Bellingham" : "Koke";
    if (strstr(ruta, "centrocampista")) return strstr(ruta, "madrid") ? "Vinicius"   : "De Paul";
    if (strstr(ruta, "mediapunta"))     return strstr(ruta, "madrid") ? "Valverde"   : "Correa";
    if (strstr(ruta, "extremo"))        return strstr(ruta, "madrid") ? "Carvajal"   : "Griezmann";
    if (strstr(ruta, "entrenador"))     return strstr(ruta, "madrid") ? "Ancelotti"  : "Simeone";
    return "?";
}

// posicion en futbol segun la ruta de textura
static const char* posicionFutbol(const char* ruta)
{
    if (!ruta || !ruta[0]) return "";
    if (strstr(ruta, "portero"))        return "Portero";
    if (strstr(ruta, "delantero"))      return "Delantero";
    if (strstr(ruta, "defensa"))        return "Defensa";
    if (strstr(ruta, "lateral"))        return "Lateral";
    if (strstr(ruta, "centrocampista")) return "Centrocampista";
    if (strstr(ruta, "mediapunta"))     return "Mediapunta";
    if (strstr(ruta, "extremo"))        return "Extremo";
    if (strstr(ruta, "entrenador"))     return "Entrenador";
    return "";
}

// pelota de futbol hecha de cuadraditos: 3x3, esquinas negras
static void dibujaBalon(double px, double py)
{
    const double s    = 4.0;   // lado de cada cuadradito
    const double paso = 5.0;   // separacion entre centros (s + 1px hueco)
    // 1 = negro, 0 = blanco — patron clasico de balon
    static const int col[3][3] = { {1,0,1}, {0,0,0}, {1,0,1} };
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            double ox = px + (c - 1) * paso - s * 0.5;
            double oy = py + (r - 1) * paso - s * 0.5;
            if (col[r][c]) glColor3d(0.05, 0.05, 0.05);
            else           glColor3d(0.95, 0.95, 0.95);
            glBegin(GL_QUADS);
                glVertex2d(ox,     oy);
                glVertex2d(ox + s, oy);
                glVertex2d(ox + s, oy + s);
                glVertex2d(ox,     oy + s);
            glEnd();
        }
    }
}

// dibuja un jugador en la arena; si rutaAtaque != nullptr la usa en lugar de la del jugador
static void dibujaJugadorArena(double cx, double cy, double tam,
                                const Jugador* pj, bool paralizado, int equipo,
                                const char* rutaAtaque = nullptr)
{
    const char* ruta = (rutaAtaque && rutaAtaque[0]) ? rutaAtaque
                       : (pj ? pj->getRutaTextura() : nullptr);
    if (ruta && ruta[0]) {
        ETSIDI::GLTexture tex = ETSIDI::getTexture(ruta);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, tex.id);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        if (paralizado) glColor4d(0.5, 0.1, 0.9, 1.0);
        else            glColor4d(1.0, 1.0, 1.0, 1.0);
        glBegin(GL_QUADS);
            glTexCoord2d(0, 1); glVertex2d(cx - tam, cy - tam);
            glTexCoord2d(1, 1); glVertex2d(cx + tam, cy - tam);
            glTexCoord2d(1, 0); glVertex2d(cx + tam, cy + tam);
            glTexCoord2d(0, 0); glVertex2d(cx - tam, cy + tam);
        glEnd();
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    } else {
        // cubo de color como fallback cuando no hay pieza asignada
        if (paralizado)       glColor3d(0.6, 0.2, 0.8);
        else if (equipo == 1) glColor3d(1.0, 1.0, 1.0);
        else                  glColor3d(0.9, 0.1, 0.1);
        glBegin(GL_QUADS);
            glVertex2d(cx - tam, cy - tam); glVertex2d(cx + tam, cy - tam);
            glVertex2d(cx + tam, cy + tam); glVertex2d(cx - tam, cy + tam);
        glEnd();
    }
}

static void dibujaArbitro(double arbY)
{
    static ETSIDI::GLTexture tex = ETSIDI::getTexture("imagenes/arbitro.png");
    double cx = 400.0;

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, tex.id);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2d(cx - 20, arbY - 25);
        glTexCoord2d(1, 1); glVertex2d(cx + 20, arbY - 25);
        glTexCoord2d(1, 0); glVertex2d(cx + 20, arbY + 50);
        glTexCoord2d(0, 0); glVertex2d(cx - 20, arbY + 50);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

// ------------------------------------------------------------
//  Inicializacion
// ------------------------------------------------------------

void Arena::inicializa()
{
    inicializa(nullptr, nullptr);
}

void Arena::inicializa(Jugador* combatiente1, Jugador* combatiente2)
{
    if (combatiente1 != nullptr && combatiente1->getEquipo() == 1) {
        pj1 = combatiente1;
        pj2 = combatiente2;
    } else {
        pj1 = combatiente2;
        pj2 = combatiente1;
    }

    j1x = -50;  j1y = 300;
    j2x = 850;  j2y = 300;
    arbY = 650;
    t = 0.0;
    tBatalla = 0.0;
    silbatoSonado = false;
    usarDiagonal = ETSIDI::lanzaMoneda();
    estado = TRANSICION;
    ganador = 0;
    equipoVentaja = 0;
    timerParalizadoJ1 = 0.0;
    timerParalizadoJ2 = 0.0;
    timerMeleeJ1 = 0.0;
    timerMeleeJ2 = 0.0;
    j1Arr = j1Aba = j1Izq = j1Der = false;
    j2Arr = j2Aba = j2Izq = j2Der = false;
    j1Ataca = false;
    j2Ataca = false;
    j1FacingIzq = false;
    j2FacingIzq = true;
    iaActiva = false;
    pausado = false;

    for (int i = 0; i < MAX_PROYECTILES; i++)
        proyectiles[i] = Proyectil();

    // Generar obstaculos: 6 barreras + 4 charcos de barro
    for (int i = 0; i < 10; i++) {
        obstaculos[i] = Obstaculo();
        obstaculos[i].tAparecer = i * 0.2;

        if (i < 6) {
            obstaculos[i].charcobool  = false;
            obstaculos[i].ancho       = 26;
            obstaculos[i].alto        = 34;
            obstaculos[i].anchoVisual = 52;
            obstaculos[i].altoVisual  = 44;
        } else {
            obstaculos[i].charcobool  = true;
            obstaculos[i].ancho       = 38;
            obstaculos[i].alto        = 30;
            obstaculos[i].anchoVisual = 38;
            obstaculos[i].altoVisual  = 30;
        }

        bool valida = false;
        for (int intento = 0; intento < 20 && !valida; intento++) {
            double px = ETSIDI::lanzaDado(670.0, 130.0);
            double py = ETSIDI::lanzaDado(470.0, 130.0);

            double dj1x = px - 200.0;  if (dj1x < 0) dj1x = -dj1x;
            double dj1y = py - 300.0;  if (dj1y < 0) dj1y = -dj1y;
            double dj2x = px - 600.0;  if (dj2x < 0) dj2x = -dj2x;
            double dj2y = py - 300.0;  if (dj2y < 0) dj2y = -dj2y;
            if (dj1x < 80 && dj1y < 80) continue;
            if (dj2x < 80 && dj2y < 80) continue;

            bool solapado = false;
            for (int j = 0; j < i; j++) {
                double ddx = px - obstaculos[j].x;  if (ddx < 0) ddx = -ddx;
                double ddy = py - obstaculos[j].y;  if (ddy < 0) ddy = -ddy;
                if (ddx < 80 && ddy < 80) { solapado = true; break; }
            }
            if (!solapado) {
                obstaculos[i].x = px;
                obstaculos[i].y = py;
                valida = true;
            }
        }
        if (!valida) {
            obstaculos[i].x = 130.0 + (i * 75) % 540;
            obstaculos[i].y = 130.0 + (i * 60) % 340;
        }
    }

    ETSIDI::playMusica("sonidos/aficion.wav", true);
}

// ------------------------------------------------------------
//  Sistema de ataque y proyectiles
// ------------------------------------------------------------

void Arena::agregaProyectil(double ox, double oy, double dx, double dy,
                             int danio, int equipo, bool atraviesa, bool paraliza,
                             double vel, double distMax)
{
    for (int i = 0; i < MAX_PROYECTILES; i++) {
        if (!proyectiles[i].activo) {
            proyectiles[i].x                   = ox;
            proyectiles[i].y                   = oy;
            proyectiles[i].dx                  = dx;
            proyectiles[i].dy                  = dy;
            proyectiles[i].vel                 = vel;
            proyectiles[i].danio               = danio;
            proyectiles[i].equipo              = equipo;
            proyectiles[i].activo              = true;
            proyectiles[i].atraviesaObstaculos = atraviesa;
            proyectiles[i].paraliza            = paraliza;
            proyectiles[i].distanciaMax        = distMax;
            proyectiles[i].distRecorrida       = 0.0;
            return;
        }
    }
    // buffer lleno: se ignora el disparo
}

void Arena::lanzaAtaque(int equipo)
{
    double ox = (equipo == 1) ? j1x : j2x;
    double oy = (equipo == 1) ? j1y : j2y;
    double tx = (equipo == 1) ? j2x : j1x;
    double ty = (equipo == 1) ? j2y : j1y;
    Jugador* pj       = (equipo == 1) ? pj1 : pj2;
    Jugador* objetivo = (equipo == 1) ? pj2 : pj1;

    // direccion hacia el oponente
    double ddx  = tx - ox,  ddy  = ty - oy;
    double dist = sqrt(ddx * ddx + ddy * ddy);
    if (dist < 1.0) return;
    ddx /= dist;  ddy /= dist;

    int    danio   = pj ? pj->getDanio()     : 10;
    bool   ranged  = pj ? pj->esRanged()     : true;
    double alcance = pj ? pj->alcanceAtaque(): 30.0;

    // sonido de disparo para ataques a distancia
    if (ranged) ETSIDI::play("sonidos/tiro.wav");

    if (equipo == equipoVentaja) danio = (int)(danio * 1.30);

    // --- habilidades especiales (tienen prioridad sobre el ataque base) ---

    // Mago/Hechicera: rayo arcano — rapido y paraliza
    if (pj && pj->getDisparaRayoArcano()) {
        agregaProyectil(ox, oy, ddx, ddy, 35, equipo, false, true, 750.0);
        return;
    }

    // Goblin: enjambre — 3 proyectiles debiles en abanico, se desvanecen a mitad del campo
    if (pj && pj->getDisparaEnjambre()) {
        double ang0       = atan2(ddy, ddx);
        double mitadCampo = (xMax - xMin) / 2.0;
        for (int a = -1; a <= 1; a++) {
            double ang = ang0 + a * (PI / 12.0);   // +-15 grados
            agregaProyectil(ox, oy, cos(ang), sin(ang), danio, equipo, false, false, 350.0, mitadCampo);
        }
        return;
    }

    // --- ataque normal ---
    if (!ranged) {
        // destello visual del swing y sonido de golpe
        if (equipo == 1) timerMeleeJ1 = 0.18;
        else             timerMeleeJ2 = 0.18;
        ETSIDI::play("sonidos/golpe.wav");
        // dano directo si el rival esta dentro del alcance del arma
        if (dist <= tam + alcance) {
            if (objetivo) objetivo->recibeGolpe(danio);
        }
    } else {
        bool atraviesa = pj ? pj->getProyectilAtraviesa() : false;
        bool paraliza  = pj ? pj->getProyectilParaliza()  : false;

        if (pj && pj->getDisparaTriple()) {
            // Manticora: tres proyectiles en abanico de +-15 grados
            double ang0    = atan2(ddy, ddx);
            double offset  = PI / 12.0;
            for (int a = -1; a <= 1; a++) {
                double ang = ang0 + a * offset;
                agregaProyectil(ox, oy, cos(ang), sin(ang),
                                danio, equipo, atraviesa, paraliza);
            }
        } else {
            agregaProyectil(ox, oy, ddx, ddy, danio, equipo, atraviesa, paraliza);
        }
    }
}

void Arena::actualizaProyectiles(double dt)
{
    for (int i = 0; i < MAX_PROYECTILES; i++) {
        if (!proyectiles[i].activo) continue;
        Proyectil& p = proyectiles[i];

        double paso = p.vel * dt;
        p.x += p.dx * paso;
        p.y += p.dy * paso;
        p.distRecorrida += paso;

        // distancia maxima superada (Goblin: mitad del campo)
        if (p.distanciaMax > 0.0 && p.distRecorrida >= p.distanciaMax) {
            p.activo = false;
            continue;
        }

        // sale del campo
        if (p.x < xMin || p.x > xMax || p.y < yMin || p.y > yMax) {
            p.activo = false;
            continue;
        }

        // colision con obstaculos (solo barreras, no charcos)
        if (!p.atraviesaObstaculos) {
            for (int j = 0; j < 8; j++) {
                if (!obstaculos[j].activo || obstaculos[j].charcobool) continue;
                double ex = p.x - obstaculos[j].x;  if (ex < 0) ex = -ex;
                double ey = p.y - obstaculos[j].y;  if (ey < 0) ey = -ey;
                if (ex < obstaculos[j].ancho / 2.0 && ey < obstaculos[j].alto / 2.0) {
                    p.activo = false;
                    break;
                }
            }
            if (!p.activo) continue;
        }

        // colision con el jugador objetivo
        double& rx = (p.equipo == 1) ? j2x : j1x;
        double& ry = (p.equipo == 1) ? j2y : j1y;
        Jugador* objetivo = (p.equipo == 1) ? pj2 : pj1;

        double ex = p.x - rx;  if (ex < 0) ex = -ex;
        double ey = p.y - ry;  if (ey < 0) ey = -ey;
        if (ex < tam && ey < tam) {
            if (objetivo) objetivo->recibeGolpe(p.danio);
            if (p.paraliza) {
                if (p.equipo == 1) timerParalizadoJ2 = 1.0;
                else               timerParalizadoJ1 = 1.0;
            }
            p.activo = false;
        }
    }
}

// ------------------------------------------------------------
//  Logica principal
// ------------------------------------------------------------

void Arena::mueve(double dt)
{
    t += dt;

    switch (estado) {
    case TRANSICION:
        if (t >= 1.0) { t = 0.0; estado = ENTRADA; }
        break;

    case ENTRADA: {
        double prog = limitacionRangoX(t / 0.8);
        double s = smoothstep(prog);
        j1x = -50.0 + s * 250.0;
        j2x = 850.0 - s * 250.0;
        if (t >= 0.8) {
            j1x = 200;  j2x = 600;
            t = 0.0;
            estado = ARBITRO_ENTRANDO;
        }
        break;
    }

    case ARBITRO_ENTRANDO: {
        double prog = limitacionRangoX(t / 0.7);
        arbY = 650.0 - smoothstep(prog) * 350.0;
        if (t >= 0.7) { arbY = 300; t = 0.0; estado = ARBITRO_ESPERA; }
        break;
    }

    case ARBITRO_ESPERA:
        if (!silbatoSonado && t >= 0.3) {
            ETSIDI::stopMusica();
            ETSIDI::play("sonidos/silbato.wav");
            silbatoSonado = true;
        }
        if (t >= 0.9) { t = 0.0; estado = ARBITRO_SALIENDO; }
        break;

    case ARBITRO_SALIENDO: {
        double prog = limitacionRangoX(t / 0.5);
        arbY = 300.0 + smoothstep(prog) * 350.0;
        if (t >= 0.5) { t = 0.0; estado = BATALLA; }
        break;
    }

    case BATALLA: {
        if (pausado) break;
        tBatalla += dt;

        // timers de paralisis y destellos mele
        if (timerParalizadoJ1 > 0.0) timerParalizadoJ1 -= dt;
        if (timerParalizadoJ2 > 0.0) timerParalizadoJ2 -= dt;
        if (timerMeleeJ1 > 0.0) timerMeleeJ1 -= dt;
        if (timerMeleeJ2 > 0.0) timerMeleeJ2 -= dt;

        // actualizar piezas: cooldown de ataque + efectos pasivos (regen Trol, etc.)
        if (pj1) pj1->actualiza(dt);
        if (pj2) pj2->actualiza(dt);

        // spawn y animacion de obstaculos
        for (int i = 0; i < 10; i++)
            obstaculos[i].actualiza(tBatalla, dt);

        double vel1 = pj1 ? pj1->getVelArena() : 200.0;
        double vel2 = pj2 ? pj2->getVelArena() : 200.0;

        // el campo solo esta listo cuando todos los obstaculos han aparecido
        bool campoListo = obstaculos[9].activo;

        // movimiento jugador 1 (WASD) — bloqueado si el campo no esta listo o esta paralizado
        if (campoListo && timerParalizadoJ1 <= 0.0) {
            if (j1Arr) j1y += vel1 * dt;
            if (j1Aba) j1y -= vel1 * dt;
            if (j1Izq) { j1x -= vel1 * dt; j1FacingIzq = true;  }
            if (j1Der) { j1x += vel1 * dt; j1FacingIzq = false; }
        }
        if (j1x - tam < xMin) j1x = xMin + tam;
        if (j1x + tam > xMax) j1x = xMax - tam;
        if (j1y - tam < yMin) j1y = yMin + tam;
        if (j1y + tam > yMax) j1y = yMax - tam;
        // los voladores pasan por encima de obstaculos
        if (!pj1 || !pj1->esVolador()) {
            for (int i = 0; i < 10; i++) {
                if (!obstaculos[i].activo) continue;
                obstaculos[i].separaJugador(j1x, j1y, tam);
            }
        }

        // IA controla j2 si esta activa
        if (iaActiva && campoListo && timerParalizadoJ2 <= 0.0) {
            j2Arr = j2Aba = j2Izq = j2Der = j2Ataca = false;
            double dx = j1x - j2x;
            double dy = j1y - j2y;
            double dist = sqrt(dx*dx + dy*dy);
            bool esRanged = pj2 && pj2->esRanged();
            double objetivo = esRanged ? 180.0 : 35.0;

            if (dist > objetivo + 15.0) {
                if (dx > 0) j2Der = true; else j2Izq = true;
                if (abs(dy) > 15.0) {
                    if (dy > 0) j2Arr = true; else j2Aba = true;
                }
            } else if (esRanged && dist < objetivo - 15.0) {
                if (dx > 0) j2Izq = true; else j2Der = true;
            }

            if (dist < (esRanged ? 350.0 : 50.0)) j2Ataca = true;
        }

        // movimiento jugador 2 (flechas) — bloqueado si el campo no esta listo o esta paralizado
        if (campoListo && timerParalizadoJ2 <= 0.0) {
            if (j2Arr) j2y += vel2 * dt;
            if (j2Aba) j2y -= vel2 * dt;
            if (j2Izq) { j2x -= vel2 * dt; j2FacingIzq = true;  }
            if (j2Der) { j2x += vel2 * dt; j2FacingIzq = false; }
        }
        if (j2x - tam < xMin) j2x = xMin + tam;
        if (j2x + tam > xMax) j2x = xMax - tam;
        if (j2y - tam < yMin) j2y = yMin + tam;
        if (j2y + tam > yMax) j2y = yMax - tam;
        // los voladores pasan por encima de obstaculos
        if (!pj2 || !pj2->esVolador()) {
            for (int i = 0; i < 10; i++) {
                if (!obstaculos[i].activo) continue;
                obstaculos[i].separaJugador(j2x, j2y, tam);
            }
        }

        // ataques: se disparan mientras se mantiene la tecla y hay cooldown disponible
        if (j1Ataca && (pj1 == nullptr || pj1->puedeAtacar())) {
            lanzaAtaque(1);
            if (pj1) pj1->reiniciaCooldown();
        }
        if (j2Ataca && (pj2 == nullptr || pj2->puedeAtacar())) {
            lanzaAtaque(2);
            if (pj2) pj2->reiniciaCooldown();
        }

        actualizaProyectiles(dt);
        interaccionArena::separa(*this);

        // comprobar muertes
        if (pj1 && !pj1->estaVivo()) {
            if (pj1->puedeRevivir()) {
                pj1->revive();
                j1x = xMin + tam + 50;  j1y = 300;
            } else {
                ganador = 2;
                estado  = FIN;
                ETSIDI::stopMusica();
            }
        }
        if (pj2 && !pj2->estaVivo()) {
            if (pj2->puedeRevivir()) {
                pj2->revive();
                j2x = xMax - tam - 50;  j2y = 300;
            } else {
                ganador = 1;
                estado  = FIN;
                ETSIDI::stopMusica();
            }
        }
        break;
    }

    case FIN:
        // partida terminada: se espera a que el coordinador vuelva al tablero
        break;
    }
}

// ------------------------------------------------------------
//  Dibujado
// ------------------------------------------------------------

void Arena::dibujaBarrasVida() const
{
    if (!pj1 || !pj2) return;

    const double barW = 200.0, barH = 14.0;
    const double y0   = 562.0;

    // barra j1 (verde) — lado izquierdo
    double r1 = (double)pj1->getHp() / pj1->getHpMax();
    glColor3d(0.25, 0.25, 0.25);
    glBegin(GL_QUADS);
        glVertex2d(60, y0); glVertex2d(60 + barW, y0);
        glVertex2d(60 + barW, y0 + barH); glVertex2d(60, y0 + barH);
    glEnd();
    glColor3d(0.2, 0.85, 0.2);
    glBegin(GL_QUADS);
        glVertex2d(60, y0); glVertex2d(60 + barW * r1, y0);
        glVertex2d(60 + barW * r1, y0 + barH); glVertex2d(60, y0 + barH);
    glEnd();

    // barra j2 (roja) — lado derecho
    double r2 = (double)pj2->getHp() / pj2->getHpMax();
    glColor3d(0.25, 0.25, 0.25);
    glBegin(GL_QUADS);
        glVertex2d(540, y0); glVertex2d(540 + barW, y0);
        glVertex2d(540 + barW, y0 + barH); glVertex2d(540, y0 + barH);
    glEnd();
    glColor3d(0.85, 0.15, 0.15);
    glBegin(GL_QUADS);
        glVertex2d(540, y0); glVertex2d(540 + barW * r2, y0);
        glVertex2d(540 + barW * r2, y0 + barH); glVertex2d(540, y0 + barH);
    glEnd();
}

void Arena::dibuja() const
{
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);  glLoadIdentity();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

    double cx = (xMin + xMax) / 2.0;
    double cy = (yMin + yMax) / 2.0;

    // fondo: imagen del estadio cubriendo toda la ventana
    {
        static ETSIDI::GLTexture texEstadio = ETSIDI::getTexture("imagenes/estadio.png");
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texEstadio.id);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glBegin(GL_QUADS);
            glTexCoord2d(0, 1); glVertex2d(0,   0);
            glTexCoord2d(1, 1); glVertex2d(800, 0);
            glTexCoord2d(1, 0); glVertex2d(800, 600);
            glTexCoord2d(0, 0); glVertex2d(0,   600);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }

    // franjas de cesped alternadas
    double anchoFranja = (xMax - xMin) / 10.0;
    for (int i = 0; i < 10; i++) {
        double x0 = xMin + i * anchoFranja;
        double x1 = x0 + anchoFranja;
        if (i % 2 == 0) glColor3d(0.13, 0.62, 0.13);
        else             glColor3d(0.10, 0.55, 0.10);
        glBegin(GL_QUADS);
            glVertex2d(x0, yMin); glVertex2d(x1, yMin);
            glVertex2d(x1, yMax); glVertex2d(x0, yMax);
        glEnd();
    }

    // lineas blancas del campo
    glColor3d(1.0, 1.0, 1.0);
    glLineWidth(2.0f);

    glBegin(GL_LINE_LOOP);
        glVertex2d(xMin, yMin); glVertex2d(xMax, yMin);
        glVertex2d(xMax, yMax); glVertex2d(xMin, yMax);
    glEnd();

    glBegin(GL_LINES);
        glVertex2d(cx, yMin); glVertex2d(cx, yMax);
    glEnd();

    dibujaCirculo(cx, cy, 70.0);
    glPointSize(6.0f);
    glBegin(GL_POINTS);
        glVertex2d(cx, cy);
    glEnd();

    // area de penalti izquierda
    glBegin(GL_LINE_LOOP);
        glVertex2d(xMin,       yMin + 110);
        glVertex2d(xMin + 160, yMin + 110);
        glVertex2d(xMin + 160, yMax - 110);
        glVertex2d(xMin,       yMax - 110);
    glEnd();

    // area pequena izquierda
    glBegin(GL_LINE_LOOP);
        glVertex2d(xMin,      yMin + 185);
        glVertex2d(xMin + 70, yMin + 185);
        glVertex2d(xMin + 70, yMax - 185);
        glVertex2d(xMin,      yMax - 185);
    glEnd();

    // area de penalti derecha
    glBegin(GL_LINE_LOOP);
        glVertex2d(xMax,       yMin + 110);
        glVertex2d(xMax - 160, yMin + 110);
        glVertex2d(xMax - 160, yMax - 110);
        glVertex2d(xMax,       yMax - 110);
    glEnd();

    // area pequena derecha
    glBegin(GL_LINE_LOOP);
        glVertex2d(xMax,      yMin + 185);
        glVertex2d(xMax - 70, yMin + 185);
        glVertex2d(xMax - 70, yMax - 185);
        glVertex2d(xMax,      yMax - 185);
    glEnd();

    glPointSize(5.0f);
    glBegin(GL_POINTS);
        glVertex2d(xMin + 110, cy);
        glVertex2d(xMax - 110, cy);
    glEnd();

    // obstaculos con texturas
    for (int i = 0; i < 10; i++)
        obstaculos[i].dibuja();

    // jugadores con su foto de personaje (morado si paralizados; sprite de ataque si estan golpeando)
    const char* ataqueJ1 = (timerMeleeJ1 > 0.0)
        ? (j1FacingIzq ? "../bin/imagenes/entrada_madrid_izquierda.png"
                        : "../bin/imagenes/entrada_madrid_derecha.png")
        : nullptr;
    const char* ataqueJ2 = (timerMeleeJ2 > 0.0)
        ? (j2FacingIzq ? "../bin/imagenes/entrada_atleti_izquierda.png"
                        : "../bin/imagenes/entrada_atleti_derecha.png")
        : nullptr;
    dibujaJugadorArena(j1x, j1y, tam, pj1, timerParalizadoJ1 > 0.0, 1, ataqueJ1);
    dibujaJugadorArena(j2x, j2y, tam, pj2, timerParalizadoJ2 > 0.0, 2, ataqueJ2);

    // proyectiles como balones de futbol (patron de cuadraditos)
    glDisable(GL_TEXTURE_2D);
    for (int i = 0; i < MAX_PROYECTILES; i++) {
        if (!proyectiles[i].activo) continue;
        dibujaBalon(proyectiles[i].x, proyectiles[i].y);
    }

    // barras de vida y nombre de pieza
    dibujaBarrasVida();

    // posicion de futbol encima de cada barra de vida
    glDisable(GL_TEXTURE_2D);
    if (pj1 != nullptr) {
        const char* pos1  = posicionFutbol(pj1->getRutaTextura());
        const char* tipo1 = pj1->esRanged() ? " [ranged]" : " [mele]";
        glColor3d(0.0, 0.0, 0.0);
        glRasterPos2d(60, 566);
        for (const char* c = pos1;  *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        for (const char* c = tipo1; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
    if (pj2 != nullptr) {
        const char* pos2  = posicionFutbol(pj2->getRutaTextura());
        const char* tipo2 = pj2->esRanged() ? " [ranged]" : " [mele]";
        glColor3d(1.0, 1.0, 1.0);
        glRasterPos2d(540, 566);
        for (const char* c = pos2;  *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        for (const char* c = tipo2; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }

    // arbitro visible durante su secuencia de entrada
    if (estado == ARBITRO_ENTRANDO || estado == ARBITRO_ESPERA || estado == ARBITRO_SALIENDO)
        dibujaArbitro(arbY);

    // overlay de transicion
    if (estado == TRANSICION) {
        double prog = smoothstep(limitacionRangoX(t / 1.0));
        if (usarDiagonal)
            dibujaDiagonal(prog);
        else
            dibujaIris(prog);
    }

    // pantalla de fin: overlay semitransparente con el ganador
    if (estado == FIN) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4d(0.0, 0.0, 0.0, 0.6);
        glBegin(GL_QUADS);
            glVertex2d(0, 0); glVertex2d(800, 0);
            glVertex2d(800, 600); glVertex2d(0, 600);
        glEnd();
        glDisable(GL_BLEND);

        if (ganador == 1) glColor3d(1.0, 1.0, 1.0);
        else              glColor3d(0.9, 0.1, 0.1);
        const char* msg = (ganador == 1) ? "VICTORIA REAL MADRID" : "VICTORIA ATLETICO";
        glRasterPos2d(270, 310);
        for (const char* c = msg; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

        glColor3d(0.6, 0.6, 0.6);
        const char* sub = "Pulsa ESC para volver al tablero";
        glRasterPos2d(260, 280);
        for (const char* c = sub; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    // overlay de pausa
    if (pausado) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4d(0.0, 0.0, 0.0, 0.45);
        glBegin(GL_QUADS);
            glVertex2d(0, 0); glVertex2d(800, 0);
            glVertex2d(800, 600); glVertex2d(0, 600);
        glEnd();
        glDisable(GL_BLEND);
        glColor3d(1.0, 0.9, 0.2);
        glRasterPos2d(340, 315);
        const char* pMsg = "PAUSA";
        for (const char* c = pMsg; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        glColor3d(0.65, 0.65, 0.65);
        glRasterPos2d(305, 290);
        const char* pSub = "Pulsa P para continuar";
        for (const char* c = pSub; *c; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glLineWidth(1.0f);
    glPointSize(1.0f);
}

// ------------------------------------------------------------
//  Entrada de teclado
// ------------------------------------------------------------

void Arena::tecla(unsigned char key)
{
    // siempre registrar el estado de las teclas
    if (key == ' ') j1Ataca = true;
    if (key == 13)  j2Ataca = true;
    if (key == 'w' || key == 'W') j1Arr = true;
    if (key == 's' || key == 'S') j1Aba = true;
    if (key == 'a' || key == 'A') j1Izq = true;
    if (key == 'd' || key == 'D') j1Der = true;

    if (estado == FIN) return;
    if (estado != BATALLA) return;

    // pausa / despausa
    if (key == 'p' || key == 'P') { pausado = !pausado; return; }

    if (pausado) return;

    // disparo inmediato al pulsar (solo si el campo esta listo)
    bool campoListo = obstaculos[9].activo;
    if (key == ' ' && campoListo) {
        if (pj1 == nullptr || pj1->puedeAtacar()) {
            lanzaAtaque(1);
            if (pj1) pj1->reiniciaCooldown();
        }
    }
    if (key == 13 && campoListo) {
        if (pj2 == nullptr || pj2->puedeAtacar()) {
            lanzaAtaque(2);
            if (pj2) pj2->reiniciaCooldown();
        }
    }
}

void Arena::teclaJ1(unsigned char key)
{
    if (key == 'w' || key == 'W') j1Arr = false;
    if (key == 's' || key == 'S') j1Aba = false;
    if (key == 'a' || key == 'A') j1Izq = false;
    if (key == 'd' || key == 'D') j1Der = false;
    if (key == ' ')               j1Ataca = false;
    if (key == 13)                j2Ataca = false;
}

void Arena::teclaEspecial(int key)
{
    if (key == GLUT_KEY_UP)    j2Arr = true;
    if (key == GLUT_KEY_DOWN)  j2Aba = true;
    if (key == GLUT_KEY_LEFT)  j2Izq = true;
    if (key == GLUT_KEY_RIGHT) j2Der = true;
}

void Arena::teclaEspecialArriba(int key)
{
    if (key == GLUT_KEY_UP)    j2Arr = false;
    if (key == GLUT_KEY_DOWN)  j2Aba = false;
    if (key == GLUT_KEY_LEFT)  j2Izq = false;
    if (key == GLUT_KEY_RIGHT) j2Der = false;
}
