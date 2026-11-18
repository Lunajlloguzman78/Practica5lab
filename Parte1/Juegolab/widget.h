#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QSoundEffect>
#include <vector>
#include "juego.h"

using namespace std;

namespace Ui {
class Widget;
}

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::Widget *ui;
    Juego juego;
    QGraphicsView *vista;
    QGraphicsScene *escena;
    QGraphicsPixmapItem *bala;
    QGraphicsPixmapItem *cañon1;
    QGraphicsPixmapItem *cañon2;
    QGraphicsPixmapItem *personaje1;
    QGraphicsPixmapItem *personaje2;
    QSlider *angulo;
    QSlider *velocidad;
    QSlider *posicion;
    QLabel *valorAngulo;
    QLabel *valorVelocidad;
    QLabel *valorPosicion;
    QPushButton *botonDisparar;
    QLabel *estado;
    QLabel *objetivo;
    QTimer reloj;
    QSoundEffect sonidoDisparo;
    QSoundEffect sonidoImpacto;
    QSoundEffect sonidoMuerte;
    bool arrastrando = false;
    vector<QGraphicsPixmapItem *> dibujosObstaculos;
    vector<QGraphicsTextItem *> textosObstaculos;
    vector<QGraphicsEllipseItem *> puntos;

    void crearEscenario();
    void actualizarPantalla();
    void actualizarTrayectoria();
    void sincronizarPosicion();
};

#endif
