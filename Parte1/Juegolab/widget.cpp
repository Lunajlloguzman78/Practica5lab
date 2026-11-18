#include "ui_widget.h"
#include "widget.h"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QPixmap>
#include <QPainter>
#include <QFont>
#include <QColor>

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    vista = ui->vista;
    angulo = ui->angulo;
    velocidad = ui->velocidad;
    posicion = ui->posicion;

    valorAngulo = ui->valorAngulo;
    valorVelocidad = ui->valorVelocidad;
    valorPosicion = ui->valorPosicion;

    botonDisparar = ui->botonDisparar;
    estado = ui->estado;
    objetivo = ui->objetivo;

    escena = new QGraphicsScene(
        0, 0,
        Juego::ancho,
        Juego::alto,
        this
        );

    vista->setScene(escena);

    vista->setHorizontalScrollBarPolicy(
        Qt::ScrollBarAlwaysOff
        );

    vista->setVerticalScrollBarPolicy(
        Qt::ScrollBarAlwaysOff
        );

    vista->setRenderHint(
        QPainter::SmoothPixmapTransform,
        true
        );

    crearEscenario();

    estado->setText("Juego de fortalezas");

    objetivo->setText(
        "Escenario inicial - Jugador 1"
        );

    setWindowTitle(
        "Juego de fortalezas - Version 1"
        );

    botonDisparar->setEnabled(false);
    angulo->setEnabled(false);
    velocidad->setEnabled(false);
    posicion->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::crearEscenario()
{
    escena->addPixmap(
        QPixmap(":/sprites/fondo.png")
        );

    personaje1 = escena->addPixmap(
        QPixmap(":/sprites/personaje1.png")
        );

    personaje1->setPos(115, 322);

    personaje2 = escena->addPixmap(
        QPixmap(":/sprites/personaje2.png")
        );

    personaje2->setPos(723, 322);

    cañon1 = escena->addPixmap(
        QPixmap(":/sprites/cañon1.png")
        );

    cañon2 = escena->addPixmap(
        QPixmap(":/sprites/cañon2.png")
        );

    cañon1->setPos(
        juego.posicion1 - 56, 337
        );

    cañon2->setPos(
        juego.posicion2 - 56, 337
        );

    for (const auto &o : juego.obstaculos)
    {
        QPixmap imagenMuro(
            ":/sprites/muro.png"
            );

        auto *imagen = escena->addPixmap(
            imagenMuro.scaled(
                o.area.size().toSize()
                )
            );

        imagen->setPos(
            o.area.topLeft()
            );

        dibujosObstaculos.push_back(imagen);

        auto *texto = escena->addText("80");

        texto->setDefaultTextColor(
            QColor("#fff4c8")
            );

        texto->setFont(
            QFont("Arial", 12, QFont::Bold)
            );

        texto->setPos(
            o.area.x() - 2,
            o.area.y() - 28
            );

        textosObstaculos.push_back(texto);
    }

    bala = escena->addPixmap(
        QPixmap(":/sprites/bala.png")
        );

    bala->hide();
}

void Widget::sincronizarPosicion()
{
}

void Widget::actualizarTrayectoria()
{
}

void Widget::actualizarPantalla()
{
}

bool Widget::eventFilter(QObject *obj, QEvent *event)
{
    return QWidget::eventFilter(obj, event);
}