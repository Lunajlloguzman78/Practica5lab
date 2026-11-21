#include "ui_widget.h"
#include "widget.h"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QPixmap>
#include <QPainter>
#include <QFont>
#include <QColor>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPen>
#include <QBrush>
#include <QUrl>
#include <QSignalBlocker>
#include <QSlider>
#include <QPushButton>
#include <QTimer>
#include <cmath>

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

    vista->viewport()->installEventFilter(this);

    sonidoDisparo.setSource(
        QUrl(QStringLiteral("qrc:/sonidos/disparo.wav"))
        );

    sonidoDisparo.setVolume(0.75);

    sonidoImpacto.setSource(
        QUrl(QStringLiteral("qrc:/sonidos/impacto.wav"))
        );

    sonidoImpacto.setVolume(0.85);

    sonidoMuerte.setSource(
        QUrl(QStringLiteral("qrc:/sonidos/muerte.wav"))
        );

    sonidoMuerte.setVolume(0.9);

    connect(
        angulo,
        &QSlider::valueChanged,
        this,
        [this](int n)
        {
            valorAngulo->setText(
                QString("Angulo: %1°").arg(n)
                );

            (juego.turno == 1
                 ? juego.jugador1
                 : juego.jugador2).angulo = n;

            actualizarPantalla();
        }
        );

    connect(
        velocidad,
        &QSlider::valueChanged,
        this,
        [this](int n)
        {
            valorVelocidad->setText(
                QString("Velocidad: %1").arg(n)
                );

            (juego.turno == 1
                 ? juego.jugador1
                 : juego.jugador2).velocidad = n;

            actualizarPantalla();
        }
        );

    connect(
        posicion,
        &QSlider::valueChanged,
        this,
        [this](int n)
        {
            juego.cambiarPosicion(
                juego.turno == 1
                    ? 40 + n
                    : 800 + n
                );

            valorPosicion->setText(
                QString("Posicion: %1").arg(n)
                );

            actualizarPantalla();
        }
        );

    connect(
        botonDisparar,
        &QPushButton::clicked,
        this,
        [this]()
        {
            if (juego.disparar(
                    angulo->value(),
                    velocidad->value()
                    ))
            {
                sonidoDisparo.play();
            }

            actualizarPantalla();
        }
        );

    connect(
        ui->reiniciar,
        &QPushButton::clicked,
        this,
        [this]()
        {
            juego.reiniciar();

            sincronizarPosicion();

            actualizarPantalla();
        }
        );

    connect(
        &reloj,
        &QTimer::timeout,
        this,
        [this]()
        {
            int turnoAnterior = juego.turno;

            const unsigned int impactosAntes =
                juego.impactos;

            const unsigned int muertesAntes =
                juego.muertes;

            juego.actualizar(0.016);

            if (juego.impactos != impactosAntes)
            {
                sonidoImpacto.play();
            }

            if (juego.muertes != muertesAntes)
            {
                sonidoMuerte.play();
            }

            if (turnoAnterior != juego.turno)
            {
                sincronizarPosicion();
            }

            actualizarPantalla();
        }
        );

    reloj.start(16);

    sincronizarPosicion();

    actualizarPantalla();
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

    auto cargarCanon = [](
                           const QString &rutaPrincipal,
                           const QString &rutaAlternativa,
                           bool invertido
                           )
    {
        QPixmap imagen(rutaPrincipal);

        if (imagen.isNull())
        {
            imagen.load(rutaAlternativa);
        }

        if (imagen.isNull())
        {
            imagen = QPixmap(112, 80);
            imagen.fill(Qt::transparent);

            QPainter painter(&imagen);

            painter.setRenderHint(
                QPainter::Antialiasing,
                true
                );

            painter.setPen(
                QPen(QColor("#202a38"), 3)
                );

            painter.setBrush(
                QBrush(QColor("#657489"))
                );

            if (invertido)
            {
                painter.drawRoundedRect(
                    8, 27, 75, 18, 6, 6
                    );
            }
            else
            {
                painter.drawRoundedRect(
                    29, 27, 75, 18, 6, 6
                    );
            }

            painter.setBrush(
                QBrush(QColor("#414d61"))
                );

            painter.drawRoundedRect(
                32, 38, 48, 22, 7, 7
                );

            painter.setBrush(
                QBrush(QColor("#273342"))
                );

            painter.drawEllipse(
                28, 51, 25, 25
                );

            painter.drawEllipse(
                62, 51, 25, 25
                );

            painter.setBrush(
                QBrush(QColor("#b8c4d2"))
                );

            painter.drawEllipse(
                36, 59, 9, 9
                );

            painter.drawEllipse(
                70, 59, 9, 9
                );
        }

        return imagen;
    };

    QPixmap imagenCanon1 = cargarCanon(
        QStringLiteral(":/sprites/canon1.png"),
        QStringLiteral(":/sprites/canon1.png"),
        false
        );

    QPixmap imagenCanon2 = cargarCanon(
        QStringLiteral(":/sprites/canon2.png"),
        QStringLiteral(":/sprites/canon2.png"),
        true
        );

    cañon1 = escena->addPixmap(
        imagenCanon1
        );

    cañon2 = escena->addPixmap(
        imagenCanon2
        );

    cañon1->setPos(
        juego.posicion1 - 56, 337
        );

    cañon2->setPos(
        juego.posicion2 - 56, 337
        );

    cañon1->setZValue(10);
    cañon2->setZValue(10);

    cañon1->setVisible(true);
    cañon2->setVisible(true);

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

    for (int i = 0; i < 32; ++i)
    {
        auto *punto = escena->addEllipse(
            0,
            0,
            5,
            5,
            QPen(Qt::NoPen),
            QBrush(
                QColor(255, 248, 205, 195)
                )
            );

        puntos.push_back(punto);
    }
}

void Widget::sincronizarPosicion()
{
    QSignalBlocker bloqueo(posicion);
    QSignalBlocker bloqueoAngulo(angulo);
    QSignalBlocker bloqueoVelocidad(velocidad);

    const Jugador &actual =
        juego.turno == 1
            ? juego.jugador1
            : juego.jugador2;

    angulo->setValue(actual.angulo);

    velocidad->setValue(actual.velocidad);

    valorAngulo->setText(
        QString("Angulo: %1°").arg(actual.angulo)
        );

    valorVelocidad->setText(
        QString("Velocidad: %1").arg(actual.velocidad)
        );

    posicion->setValue(
        int(juego.posicionActual()) -
        (juego.turno == 1 ? 40 : 800)
        );

    valorPosicion->setText(
        QString("Posicion: %1").arg(posicion->value())
        );
}

void Widget::actualizarTrayectoria()
{
    if (juego.proyectil.activo ||
        juego.ganador != 0)
    {
        for (auto *p : puntos)
        {
            p->hide();
        }

        return;
    }

    const double rad =
        angulo->value() *
        3.141592653589793 / 180.0;

    const double vx =
        (juego.turno == 1 ? 1 : -1) *
        velocidad->value() *
        std::cos(rad);

    const double vy =
        -velocidad->value() *
        std::sin(rad);

    for (int i = 0; i < int(puntos.size()); ++i)
    {
        double t = (i + 1) * 0.095;

        double x =
            juego.posicionActual() +
            vx * t;

        double y =
            365 +
            vy * t +
            85 * t * t;

        bool visible =
            x >= 5 &&
            x <= 895 &&
            y >= 5 &&
            y < Juego::suelo;

        puntos[i]->setVisible(visible);

        if (visible)
        {
            puntos[i]->setPos(x, y);
        }
    }
}

void Widget::actualizarPantalla()
{
    estado->setText(juego.mensaje);

    setWindowTitle(
        QString("Juego de fortalezas - Jugador %1")
            .arg(juego.turno)
        );

    bool controlesActivos =
        !juego.proyectil.activo &&
        juego.ganador == 0;

    botonDisparar->setEnabled(controlesActivos);

    angulo->setEnabled(controlesActivos);

    velocidad->setEnabled(controlesActivos);

    posicion->setEnabled(controlesActivos);

    bala->setVisible(juego.proyectil.activo);

    if (juego.proyectil.activo)
    {
        bala->setPos(
            juego.proyectil.x - 9,
            juego.proyectil.y - 9
            );
    }

    cañon1->setPos(
        juego.posicion1 - 56,
        337
        );

    cañon2->setPos(
        juego.posicion2 - 56,
        337
        );

    cañon1->setRotation(0);

    cañon2->setRotation(0);

    personaje1->setVisible(
        juego.jugador1.vivo
        );

    personaje2->setVisible(
        juego.jugador2.vivo
        );

    for (size_t i = 0;
         i < juego.obstaculos.size();
         ++i)
    {
        const auto &o = juego.obstaculos[i];

        dibujosObstaculos[i]->setVisible(
            !o.destruido()
            );

        textosObstaculos[i]->setVisible(
            !o.destruido()
            );

        textosObstaculos[i]->setPlainText(
            QString::number(
                int(std::ceil(o.resistencia))
                )
            );
    }

    if (juego.ganador != 0)
    {
        objetivo->setText(
            "Partida terminada. "
            "Pulsa Reiniciar para volver a jugar."
            );
    }
    else
    {
        int defensasJ1 =
            int(!juego.obstaculos[0].destruido()) +
            int(!juego.obstaculos[1].destruido());

        int defensasJ2 =
            int(!juego.obstaculos[2].destruido()) +
            int(!juego.obstaculos[3].destruido());

        objetivo->setText(
            QString(
                "Defensas J1: %1/2     |     "
                "Defensas J2: %2/2     |     "
                "Arrastra el Cañon para moverlo"
                )
                .arg(defensasJ1)
                .arg(defensasJ2)
            );
    }

    actualizarTrayectoria();
}

bool Widget::eventFilter(
    QObject *obj,
    QEvent *event
    )
{
    if (obj != vista->viewport())
    {
        return QWidget::eventFilter(
            obj,
            event
            );
    }

    if (juego.proyectil.activo ||
        juego.ganador != 0)
    {
        arrastrando = false;

        return QWidget::eventFilter(
            obj,
            event
            );
    }

    if (event->type() == QEvent::MouseButtonPress)
    {
        auto *mouse =
            static_cast<QMouseEvent *>(event);

        QPointF punto =
            vista->mapToScene(mouse->pos());

        if (mouse->button() == Qt::LeftButton &&
            std::abs(
                punto.x() -
                juego.posicionActual()
                ) < 48 &&
            punto.y() > 330 &&
            punto.y() < 412)
        {
            arrastrando = true;

            return true;
        }
    }

    if (event->type() == QEvent::MouseMove &&
        arrastrando)
    {
        auto *mouse =
            static_cast<QMouseEvent *>(event);

        QPointF punto =
            vista->mapToScene(mouse->pos());

        juego.cambiarPosicion(punto.x());

        sincronizarPosicion();

        actualizarPantalla();

        return true;
    }

    if (event->type() == QEvent::MouseButtonRelease)
    {
        arrastrando = false;
    }

    return QWidget::eventFilter(obj, event);
}