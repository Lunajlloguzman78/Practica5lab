#include "proyectil.h"
#include <cmath>
void Proyectil::lanzar(double inicioX, double inicioY, double angulo, double velocidad, int direccion) {
    const double radianes = angulo * 3.141592653589793 / 180.0;
    x = inicioX;
    y = inicioY;
    vx = direccion * velocidad * std::cos(radianes);
    vy = -velocidad * std::sin(radianes);
    activo = true;
}
void Proyectil::mover(double dt) {
    vy += 170 * dt;
    x += vx * dt;
    y += vy * dt;
}
double Proyectil::rapidez() const { return std::sqrt(vx * vx + vy * vy); }