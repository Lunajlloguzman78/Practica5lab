#include "Simulacion.h"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;


Simulacion::Simulacion(double ancho, double alto, double restitucion)
    : ancho(ancho),
    alto(alto),
    restitucion(restitucion),
    tiempo(0),
    cantidadColisiones(0) {
}
void Simulacion::agregarParticula(const Particula &p) {

    particulas.push_back(p);
}

void Simulacion::agregarObstaculo(const Obstaculo &o) {

    obstaculos.push_back(o);
}

void Simulacion::registrar(const string &tipo, int a, int b) {

    colisiones << fixed << setprecision(4)
    << tiempo << ','
    << tipo << ','
    << a << ','
    << b << '\n';

    cantidadColisiones++;
}

void Simulacion::paredes(Particula &p) {

    if (p.x - p.radio < 0 && p.vx < 0) {

        p.x = p.radio;
        p.vx = -p.vx;

        registrar("pared_izquierda", p.id, 0);

    } else if (p.x + p.radio > ancho && p.vx > 0) {

        p.x = ancho - p.radio;
        p.vx = -p.vx;

        registrar("pared_derecha", p.id, 0);
    }

    if (p.y - p.radio < 0 && p.vy < 0) {

        p.y = p.radio;
        p.vy = -p.vy;

        registrar("pared_inferior", p.id, 0);

    } else if (p.y + p.radio > alto && p.vy > 0) {

        p.y = alto - p.radio;
        p.vy = -p.vy;

        registrar("pared_superior", p.id, 0);
    }
}

void Simulacion::chocarObstaculo(Particula &p, const Obstaculo &o) {

    double izquierda = o.x - o.lado / 2;
    double derecha = o.x + o.lado / 2;
    double abajo = o.y - o.lado / 2;
    double arriba = o.y + o.lado / 2;

    double cx = clamp(p.x, izquierda, derecha);
    double cy = clamp(p.y, abajo, arriba);

    double dx = p.x - cx;
    double dy = p.y - cy;

    double distancia2 = dx * dx + dy * dy;

    if (distancia2 >= p.radio * p.radio)
        return;

    double nx = 0;
    double ny = 0;
    double penetracion = 0;

    if (distancia2 > 1e-12) {

        double distancia = sqrt(distancia2);

        nx = dx / distancia;
        ny = dy / distancia;

        penetracion = p.radio - distancia;

    } else {

        double dIzq = p.x - izquierda;
        double dDer = derecha - p.x;
        double dAbajo = p.y - abajo;
        double dArriba = arriba - p.y;

        double menor = min({
            dIzq,
            dDer,
            dAbajo,
            dArriba
        });

        if (menor == dIzq)
            nx = -1;
        else if (menor == dDer)
            nx = 1;
        else if (menor == dAbajo)
            ny = -1;
        else
            ny = 1;

        penetracion = p.radio + menor;
    }

    p.x += nx * penetracion;
    p.y += ny * penetracion;

    double normal = p.vx * nx + p.vy * ny;

    if (normal < 0) {

        p.vx -= (1 + restitucion) * normal * nx;
        p.vy -= (1 + restitucion) * normal * ny;

        registrar("obstaculo", p.id, o.id);
    }
}


void Simulacion::unirParticulas() {

    for (size_t i = 0; i < particulas.size(); ++i) {

        for (size_t j = i + 1; j < particulas.size();) {

            auto &a = particulas[i];
            auto &b = particulas[j];

            double dx = b.x - a.x;
            double dy = b.y - a.y;

            double radios = a.radio + b.radio;

            if (dx * dx + dy * dy <= radios * radios) {

                double masa = a.masa + b.masa;

                double x = (a.x * a.masa + b.x * b.masa) / masa;
                double y = (a.y * a.masa + b.y * b.masa) / masa;

                double vx = (a.vx * a.masa + b.vx * b.masa) / masa;
                double vy = (a.vy * a.masa + b.vy * b.masa) / masa;

                double radio = sqrt(
                    a.radio * a.radio + b.radio * b.radio
                    );

                registrar("union_particulas", a.id, b.id);

                a.x = x;
                a.y = y;

                a.vx = vx;
                a.vy = vy;

                a.masa = masa;
                a.radio = radio;

                particulas.erase(
                    particulas.begin() + static_cast<ptrdiff_t>(j)
                    );

                paredes(particulas[i]);

                for (const auto &o : obstaculos) {
                    chocarObstaculo(particulas[i], o);
                }

                j = i + 1;

            } else {

                ++j;
            }
        }
    }
}


void Simulacion::guardarPosiciones() {

    for (const auto &p : particulas) {

        posiciones << fixed << setprecision(4)
        << tiempo << ','
        << p.id << ','
        << p.x << ','
        << p.y << ','
        << p.vx << ','
        << p.vy << ','
        << p.masa << ','
        << p.radio << '\n';
    }
}


void Simulacion::crearGrafica(const string &ruta) {

    ifstream archivo(ruta + "/posiciones.txt");
    ofstream svg(ruta + "/trayectorias.svg");

    if (!archivo || !svg)
        return;

    const int margen = 65;
    const int escala = 7;

    const int w = static_cast<int>(ancho * escala) + 2 * margen;
    const int h = static_cast<int>(alto * escala) + 2 * margen;

    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\""
        << w << "\" height=\"" << h
        << "\" viewBox=\"0 0 " << w << ' ' << h << "\">\n";

    svg << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";

    svg << "<text x=\"" << w / 2
        << "\" y=\"30\" text-anchor=\"middle\" font-size=\"19\">"
        << "Trayectorias de las particulas</text>\n";

    svg << "<rect x=\"" << margen
        << "\" y=\"" << margen
        << "\" width=\"" << ancho * escala
        << "\" height=\"" << alto * escala
        << "\" fill=\"#f8fafc\" stroke=\"#334155\" stroke-width=\"2\"/>\n";

    for (const auto &o : obstaculos) {

        svg << "<rect x=\""
            << margen + (o.x - o.lado / 2) * escala
            << "\" y=\""
            << margen + (alto - o.y - o.lado / 2) * escala
            << "\" width=\"" << o.lado * escala
            << "\" height=\"" << o.lado * escala
            << "\" fill=\"#94a3b8\" stroke=\"#475569\"/>\n";
    }

    string linea;

    getline(archivo, linea);

    const char *colores[] = {
        "#2563eb",
        "#dc2626",
        "#16a34a",
        "#9333ea",
        "#ea580c",
        "#0891b2"
    };

    while (getline(archivo, linea)) {

        stringstream ss(linea);
        string dato;

        getline(ss, dato, ',');

        getline(ss, dato, ',');
        int id = stoi(dato);

        getline(ss, dato, ',');
        double x = stod(dato);

        getline(ss, dato, ',');
        double y = stod(dato);

        svg << "<circle cx=\""
            << margen + x * escala
            << "\" cy=\""
            << margen + (alto - y) * escala
            << "\" r=\"1.2\" fill=\""
            << colores[(id - 1) % 6]
            << "\"/>\n";
    }

    svg << "<text x=\"" << w / 2
        << "\" y=\"" << h - 12
        << "\" text-anchor=\"middle\" font-size=\"14\">"
        << "Posicion X</text>\n";

    svg << "<text x=\"20\" y=\"" << h / 2
        << "\" transform=\"rotate(-90 20 " << h / 2
        << ")\" text-anchor=\"middle\" font-size=\"14\">"
        << "Posicion Y</text>\n";

    svg << "</svg>\n";
}

bool Simulacion::ejecutar(
    double duracion,
    double dt,
    const string &carpeta
    ) {

    if (dt <= 0 ||
        duracion <= 0 ||
        restitucion < 0 ||
        restitucion >= 1 ||
        particulas.size() < 4 ||
        obstaculos.size() != 4) {

        return false;
    }

    filesystem::create_directories(carpeta);

    posiciones.open(carpeta + "/posiciones.txt");
    colisiones.open(carpeta + "/colisiones.txt");

    if (!posiciones || !colisiones)
        return false;

    posiciones << "tiempo,id,x,y,vx,vy,masa,radio\n";
    colisiones << "tiempo,tipo,particula,otro_objeto\n";

    guardarPosiciones();

    int pasos = static_cast<int>(ceil(duracion / dt));

    for (int paso = 1; paso <= pasos; ++paso) {

        double avance = min(dt, duracion - tiempo);

        if (avance <= 0)
            break;

        tiempo += avance;

        for (auto &p : particulas) {

            p.mover(avance);

            paredes(p);

            for (const auto &o : obstaculos) {
                chocarObstaculo(p, o);
            }
        }

        unirParticulas();

        guardarPosiciones();
    }

    posiciones.close();
    colisiones.close();

    crearGrafica(carpeta);

    cout << "Simulacion terminada\n";

    cout << "Tiempo: "
         << tiempo << " segundos\n";

    cout << "Particulas finales: "
         << particulas.size() << "\n";

    cout << "Colisiones registradas: "
         << cantidadColisiones << "\n";

    cout << "Resultados guardados en: "
         << filesystem::absolute(carpeta) << '\n';

    return true;
}
