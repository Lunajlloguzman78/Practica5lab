#ifndef SIMULACION_H
#define SIMULACION_H

#include "Particula.h"
#include "Obstaculo.h"
#include <fstream>
#include <string>
#include <vector>

class Simulacion {
    double ancho, alto, restitucion, tiempo;
    std::vector<Particula> particulas;
    std::vector<Obstaculo> obstaculos;
    std::ofstream posiciones, colisiones;
    int cantidadColisiones;

    void paredes(Particula &p);
    void chocarObstaculo(Particula &p, const Obstaculo &o);
    void unirParticulas();
    void registrar(const std::string &tipo, int a, int b);
    void guardarPosiciones();
    void crearGrafica(const std::string &ruta);

public:
    Simulacion(double ancho, double alto, double restitucion);
    void agregarParticula(const Particula &p);
    void agregarObstaculo(const Obstaculo &o);
    bool ejecutar(double duracion, double dt, const std::string &carpeta);
};
#endif
