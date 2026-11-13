#ifndef JUGADOR_H
#define JUGADOR_H
class Jugador {
public:
    int numero;
    bool vivo = true;
    int angulo = 35;
    int velocidad = 300;
    explicit Jugador(int n);
};
#endif
