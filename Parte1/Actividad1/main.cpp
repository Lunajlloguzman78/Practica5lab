#include "Simulacion.h"
#include <iostream>

int main() {
    Simulacion simulacion(100, 70, 0.75);

    simulacion.agregarParticula(Particula(1, 12, 15, 12, 7, 2, 2));
    simulacion.agregarParticula(Particula(2, 88, 15, -12, 7, 3, 2.5));
    simulacion.agregarParticula(Particula(3, 12, 55, 11, -6, 2, 2));
    simulacion.agregarParticula(Particula(4, 88, 55, -11, -6, 4, 3));

    simulacion.agregarObstaculo(Obstaculo(1, 30, 20, 10));
    simulacion.agregarObstaculo(Obstaculo(2, 70, 20, 10));
    simulacion.agregarObstaculo(Obstaculo(3, 30, 50, 10));
    simulacion.agregarObstaculo(Obstaculo(4, 70, 50, 10));

    if (!simulacion.ejecutar(30, 0.01, "resultados")) {
        std::cerr << "No se pudo ejecutar la simulacion\n";
        return 1;
    }
    return 0;
}