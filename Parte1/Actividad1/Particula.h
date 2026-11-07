#ifndef PARTICULA_H
#define PARTICULA_H
class Particula {
public:
    int id;
    double x, y, vx, vy, masa, radio;
    Particula(int id, double x, double y, double vx, double vy, double masa, double radio);
    void mover(double dt);
};

#endif