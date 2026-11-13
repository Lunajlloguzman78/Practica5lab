#ifndef OBSTACULO_H
#define OBSTACULO_H
#include <QRectF>
class Obstaculo {
public:
    QRectF area;
    int dueno;
    double resistencia;
    Obstaculo(QRectF zona, int jugador, double vida);
    bool destruido() const;
    void recibirDanio(double cantidad);
};
#endif
