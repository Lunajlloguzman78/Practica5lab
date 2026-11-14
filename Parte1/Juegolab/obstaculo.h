#ifndef OBSTACULO_H
#define OBSTACULO_H
#include <QRectF>
class Obstaculo {
public:
    QRectF area;
    int dueño;
    double resistencia;
    Obstaculo(QRectF zona, int jugador, double vida);
    bool destruido() const;
    void recibirDaño(double cantidad);
};
#endif
