#ifndef JUEGO_H
#define JUEGO_H
#include "proyectil.h"
#include "obstaculo.h"
#include "jugador.h"
#include <vector>
#include <QString>
class Juego {
public:
    static constexpr int ancho = 900;
    static constexpr int alto = 490;
    static constexpr int suelo = 410;
    Proyectil proyectil;
    std::vector<Obstaculo> obstaculos;
    Jugador jugador1{1};
    Jugador jugador2{2};
    int turno = 1;
    int ganador = 0;
    unsigned int impactos = 0;
    unsigned int muertes = 0;
    double posicion1 = 65;
    double posicion2 = 835;
    QString mensaje;
    Juego();
    void reiniciar();
    bool disparar(double angulo, double velocidad);
    void actualizar(double dt);
    bool defensasDestruidas(int jugador) const;
    double posicionActual() const;
    void cambiarPosicion(double x);
private:
    double tiempoDisparo = 0;
    void terminarTurno();
    void rebotarParedes();
    void revisarObstaculos();
    void revisarRepresentantes();
};
#endif
