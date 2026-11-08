#include "Particula.h"

Particula::Particula(int id, double x, double y, double vx, double vy, double masa, double radio)
    : id(id), x(x), y(y), vx(vx), vy(vy), masa(masa), radio(radio) {}

void Particula::mover(double dt) {
    x += vx * dt;
    y += vy * dt;
}