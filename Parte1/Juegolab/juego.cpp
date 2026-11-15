#include "juego.h"
#include <cmath>
#include <algorithm>
Juego::Juego() { reiniciar(); }
void Juego::reiniciar() {
    jugador1.vivo = true;
    jugador1.angulo = 35;
    jugador1.velocidad = 300;
    jugador2.vivo = true;
    jugador2.angulo = 35;
    jugador2.velocidad = 300;
    turno = 1;
    ganador = 0;
    tiempoDisparo = 0;
    posicion1 = 65;
    posicion2 = 835;
    proyectil.activo = false;
    obstaculos.clear();
    obstaculos.emplace_back(QRectF(195, 332, 42, 78), 1, 80);
    obstaculos.emplace_back(QRectF(250, 352, 42, 58), 1, 80);
    obstaculos.emplace_back(QRectF(608, 352, 42, 58), 2, 80);
    obstaculos.emplace_back(QRectF(663, 332, 42, 78), 2, 80);
    mensaje = "Turno del jugador 1";
}
double Juego::posicionActual() const { return turno == 1 ? posicion1 : posicion2; }
void Juego::cambiarPosicion(double x) {
    if (proyectil.activo || ganador != 0) return;
    if (turno == 1) posicion1 = std::clamp(x, 40.0, 100.0);
    else posicion2 = std::clamp(x, 800.0, 860.0);
}
bool Juego::disparar(double angulo, double velocidad) {
    if (proyectil.activo || ganador != 0) return false;
    proyectil.lanzar(posicionActual(), 365, angulo, velocidad, turno == 1 ? 1 : -1);
    tiempoDisparo = 0;
    mensaje = QString("Dispara el jugador %1").arg(turno);
    return true;
}
void Juego::terminarTurno() {
    proyectil.activo = false;
    if (ganador != 0) return;
    turno = turno == 1 ? 2 : 1;
    mensaje = QString("Turno del jugador %1").arg(turno);
}
void Juego::rebotarParedes() {
    auto &p = proyectil;
    if (p.x < p.radio && p.vx < 0) { p.x = p.radio; p.vx = -p.vx; }
    if (p.x > ancho - p.radio && p.vx > 0) { p.x = ancho - p.radio; p.vx = -p.vx; }
    if (p.y < p.radio && p.vy < 0) { p.y = p.radio; p.vy = -p.vy; }
    if (p.y > suelo - p.radio && p.vy > 0) { p.y = suelo - p.radio; p.vy = -p.vy; }
}
void Juego::revisarObstaculos() {
    auto &p = proyectil;
    for (auto &o : obstaculos) {
        if (o.destruido() || o.dueño == turno) continue;
        double cx = std::clamp(p.x, o.area.left(), o.area.right());
        double cy = std::clamp(p.y, o.area.top(), o.area.bottom());
        double dx = p.x - cx;
        double dy = p.y - cy;
        if (dx * dx + dy * dy > p.radio * p.radio) continue;
        double nx = 0, ny = 0;
        double distancia = std::sqrt(dx * dx + dy * dy);
        if (distancia > 0.001) { nx = dx / distancia; ny = dy / distancia; }
        else {
            double izquierda = std::abs(p.x - o.area.left());
            double derecha = std::abs(o.area.right() - p.x);
            double arriba = std::abs(p.y - o.area.top());
            double abajo = std::abs(o.area.bottom() - p.y);
            double minimo = std::min({izquierda, derecha, arriba, abajo});
            if (minimo == izquierda) nx = -1;
            else if (minimo == derecha) nx = 1;
            else if (minimo == arriba) ny = -1;
            else ny = 1;
        }
        double normal = p.vx * nx + p.vy * ny;
        if (normal >= 0) continue;
        double rapidez = p.rapidez();
        double danio = 0.20 * p.masa * rapidez;
        o.recibirDanio(danio);
        ++impactos;
        const double e = 0.45;
        p.vx -= (1.0 + e) * normal * nx;
        p.vy -= (1.0 + e) * normal * ny;
        p.x = cx + nx * (p.radio + 1);
        p.y = cy + ny * (p.radio + 1);
        mensaje = QString("Impacto: %1 de resistencia").arg(int(std::ceil(o.resistencia)));
        return;
    }
}