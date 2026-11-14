#include "obstaculo.h"
#include <algorithm>
Obstaculo::Obstaculo(QRectF zona, int jugador, double vida) : area(zona), dueño(jugador), resistencia(vida) {}
bool Obstaculo::destruido() const { return resistencia <= 0; }
void Obstaculo::recibirDaño(double cantidad) { resistencia = std::max(0.0, resistencia - cantidad); }