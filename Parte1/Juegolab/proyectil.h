#ifndef PROYECTIL_H
#define PROYECTIL_H
class Proyectil {
public:
    double x = 0, y = 0, vx = 0, vy = 0;
    double masa = 2, radio = 9;
    bool activo = false;
    void lanzar(double inicioX, double inicioY, double angulo, double velocidad, int direccion);
    void mover(double dt);
    double rapidez() const;
};
#endif
