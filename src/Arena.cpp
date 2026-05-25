#include "Arena.h"
#include "interaccionArena.h"
#include "ETSIDI.h"
#include "freeglut.h"
#include <cmath>

static const double PI = 3.14159265358979;

static double limitacionRangoX(double x) {
    if (x < 0.0) return 0.0;
    if (x > 1.0) return 1.0;
    return x;
}

// suavizado: lento al inicio y al final
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
// prog=0: cubre toda la pantalla; prog=1: ha salido por la derecha
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

// iris que se abre desde el centro: prog=0 todo negro, prog=1 todo revelado
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

// empuja al jugador (px,py) fuera del obstaculo o usando AABB
static void separaDeObstaculo(double& px, double& py, double radio, const Obstaculo& o)
{
    double dx = px - o.x;  if (dx < 0) dx = -dx;
    double dy = py - o.y;  if (dy < 0) dy = -dy;
    double overlapX = (radio + o.ancho / 2.0) - dx;
    double overlapY = (radio + o.alto  / 2.0) - dy;
    if (overlapX > 0 && overlapY > 0) {
        if (overlapX < overlapY)
            px += (px > o.x ? overlapX : -overlapX);
        else
            py += (py > o.y ? overlapY : -overlapY);
    }
}

void Arena::inicializa()
{
    j1x = -50;  j1y = 300;
    j2x = 850;  j2y = 300;
    arbY = 650;
    t = 0.0;
    tBatalla = 0.0;
    silbatoSonado = false;
    usarDiagonal = ETSIDI::lanzaMoneda();
    estado = TRANSICION;

    // Generar obstaculos: 5 barreras + 3 charcos de barro
    for (int i = 0; i < 8; i++) {
        obstaculos[i] = Obstaculo();
        obstaculos[i].tAparecer = i * 0.2;  // aparecen de 0.0 a 1.4 s desde el inicio de BATALLA

        if (i < 5) {
            obstaculos[i].esBanderin  = false;  // barrera
            obstaculos[i].ancho       = 35;     // hitbox (otro tercio menos)
            obstaculos[i].alto        = 45;
            obstaculos[i].anchoVisual = 69;     // visual (30% mas grande que el anterior)
            obstaculos[i].altoVisual  = 59;
        } else {
            obstaculos[i].esBanderin  = true;   // charco de barro
            obstaculos[i].ancho       = 50;
            obstaculos[i].alto        = 40;
            obstaculos[i].anchoVisual = 50;
            obstaculos[i].altoVisual  = 40;
        }

        // Posicion aleatoria dentro del campo con margen
        bool valida = false;
        for (int intento = 0; intento < 20 && !valida; intento++) {
            double px = ETSIDI::lanzaDado(670.0, 130.0);
            double py = ETSIDI::lanzaDado(470.0, 130.0);

            // Evitar zona inicial de los jugadores
            double dj1x = px - 200.0;  if (dj1x < 0) dj1x = -dj1x;
            double dj1y = py - 300.0;  if (dj1y < 0) dj1y = -dj1y;
            double dj2x = px - 600.0;  if (dj2x < 0) dj2x = -dj2x;
            double dj2y = py - 300.0;  if (dj2y < 0) dj2y = -dj2y;
            if (dj1x < 80 && dj1y < 80) continue;
            if (dj2x < 80 && dj2y < 80) continue;

            // Evitar solapamiento con obstaculos ya colocados
            bool solapado = false;
            for (int j = 0; j < i; j++) {
                double ddx = px - obstaculos[j].x;  if (ddx < 0) ddx = -ddx;
                double ddy = py - obstaculos[j].y;  if (ddy < 0) ddy = -ddy;
                if (ddx < 100 && ddy < 100) { solapado = true; break; }
            }
            if (!solapado) {
                obstaculos[i].x = px;
                obstaculos[i].y = py;
                valida = true;
            }
        }
        if (!valida) {
            // Posicion de respaldo si no se encontro valida
            obstaculos[i].x = 130.0 + (i * 95) % 540;
            obstaculos[i].y = 130.0 + (i * 75) % 340;
        }
    }

    ETSIDI::playMusica("sonidos/aficion.wav", true);
}

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
        j1x = -50.0 + s * 250.0;    // -50 -> 200
        j2x = 850.0 - s * 250.0;    // 850 -> 600
        if (t >= 0.8) {
            j1x = 200;  j2x = 600;
            t = 0.0;
            estado = ARBITRO_ENTRANDO;
        }
        break;
    }

    case ARBITRO_ENTRANDO: {
        double prog = limitacionRangoX(t / 0.7);
        arbY = 650.0 - smoothstep(prog) * 350.0;   // 650 -> 300
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
        arbY = 300.0 + smoothstep(prog) * 350.0;   // 300 -> 650
        if (t >= 0.5) { t = 0.0; estado = BATALLA; }
        break;
    }

    case BATALLA: {
        tBatalla += dt;

        // Spawn y animacion de obstaculos
        for (int i = 0; i < 8; i++) {
            if (!obstaculos[i].activo && tBatalla >= obstaculos[i].tAparecer)
                obstaculos[i].activo = true;
            if (obstaculos[i].activo)
                obstaculos[i].tVida += dt;
        }

        const double vel = 200.0;   // pixeles por segundo

        // Movimiento jugador 1 (WASD)
        if (j1Arr) j1y += vel * dt;
        if (j1Aba) j1y -= vel * dt;
        if (j1Izq) j1x -= vel * dt;
        if (j1Der) j1x += vel * dt;
        if (j1x - tam < xMin) j1x = xMin + tam;
        if (j1x + tam > xMax) j1x = xMax - tam;
        if (j1y - tam < yMin) j1y = yMin + tam;
        if (j1y + tam > yMax) j1y = yMax - tam;
        for (int i = 0; i < 8; i++) {
            if (!obstaculos[i].activo) continue;
            separaDeObstaculo(j1x, j1y, tam, obstaculos[i]);
        }

        // Movimiento jugador 2 (flechas)
        if (j2Arr) j2y += vel * dt;
        if (j2Aba) j2y -= vel * dt;
        if (j2Izq) j2x -= vel * dt;
        if (j2Der) j2x += vel * dt;
        if (j2x - tam < xMin) j2x = xMin + tam;
        if (j2x + tam > xMax) j2x = xMax - tam;
        if (j2y - tam < yMin) j2y = yMin + tam;
        if (j2y + tam > yMax) j2y = yMax - tam;
        for (int i = 0; i < 8; i++) {
            if (!obstaculos[i].activo) continue;
            separaDeObstaculo(j2x, j2y, tam, obstaculos[i]);
        }

        interaccionArena::separa(*this);
        break;
    }
    }
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

    // obstaculos del campo con texturas (barreras y charcos de barro)
    {
        static ETSIDI::GLTexture texBarrera = ETSIDI::getTexture("imagenes/barrera.png");
        static ETSIDI::GLTexture texBarro   = ETSIDI::getTexture("imagenes/barro.png");

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (int i = 0; i < 8; i++) {
            if (!obstaculos[i].activo) continue;
            // animacion de aparicion: escala de 0 a 1 en 0.3 segundos
            double escala = limitacionRangoX(obstaculos[i].tVida / 0.3);
            double a2 = obstaculos[i].anchoVisual * escala / 2.0;
            double b2 = obstaculos[i].altoVisual  * escala / 2.0;
            double ox = obstaculos[i].x;
            double oy = obstaculos[i].y;

            if (obstaculos[i].esBanderin) {
                // charco de barro
                glBindTexture(GL_TEXTURE_2D, texBarro.id);
                glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                glColor3d(1.0, 1.0, 1.0);
                glBegin(GL_QUADS);
                    glTexCoord2d(0, 0); glVertex2d(ox - a2, oy - b2);
                    glTexCoord2d(1, 0); glVertex2d(ox + a2, oy - b2);
                    glTexCoord2d(1, 1); glVertex2d(ox + a2, oy + b2);
                    glTexCoord2d(0, 1); glVertex2d(ox - a2, oy + b2);
                glEnd();
            } else {
                // barrera: GL_REPLACE para no teñir de blanco, coordenadas volteadas
                glBindTexture(GL_TEXTURE_2D, texBarrera.id);
                glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                glBegin(GL_QUADS);
                    glTexCoord2d(0, 1); glVertex2d(ox - a2, oy - b2);
                    glTexCoord2d(1, 1); glVertex2d(ox + a2, oy - b2);
                    glTexCoord2d(1, 0); glVertex2d(ox + a2, oy + b2);
                    glTexCoord2d(0, 0); glVertex2d(ox - a2, oy + b2);
                glEnd();
                glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            }
        }

        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    }

    // jugadores en sus posiciones actuales
    glColor3d(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        glVertex2d(j1x - tam, j1y - tam); glVertex2d(j1x + tam, j1y - tam);
        glVertex2d(j1x + tam, j1y + tam); glVertex2d(j1x - tam, j1y + tam);
    glEnd();

    glColor3d(0.9, 0.1, 0.1);
    glBegin(GL_QUADS);
        glVertex2d(j2x - tam, j2y - tam); glVertex2d(j2x + tam, j2y - tam);
        glVertex2d(j2x + tam, j2y + tam); glVertex2d(j2x - tam, j2y + tam);
    glEnd();

    // arbitro visible durante su secuencia de entrada
    if (estado == ARBITRO_ENTRANDO || estado == ARBITRO_ESPERA || estado == ARBITRO_SALIENDO)
        dibujaArbitro(arbY);

    // overlay de transicion encima de todo (diagonal o iris, elegido al azar)
    if (estado == TRANSICION) {
        double prog = smoothstep(limitacionRangoX(t / 1.0));
        if (usarDiagonal)
            dibujaDiagonal(prog);
        else
            dibujaIris(prog);
    }

    glLineWidth(1.0f);
    glPointSize(1.0f);
}

void Arena::tecla(unsigned char key)
{
    if (estado != BATALLA) return;

    // actualizar flags de teclas pulsadas
    if (key == 'w' || key == 'W') j1Arr = true;
    if (key == 's' || key == 'S') j1Aba = true;
    if (key == 'a' || key == 'A') j1Izq = true;
    if (key == 'd' || key == 'D') j1Der = true;
}

void Arena::teclaJ1(unsigned char key)
{
    if (key == 'w' || key == 'W') j1Arr = false;
    if (key == 's' || key == 'S') j1Aba = false;
    if (key == 'a' || key == 'A') j1Izq = false;
    if (key == 'd' || key == 'D') j1Der = false;
}

void Arena::teclaEspecial(int key)
{
    if (estado != BATALLA) return;

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
