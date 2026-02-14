#ifndef JUGADOR_H
#define JUGADOR_H
#include <string>
class Jugador
{
public:
    Jugador();
    void setNombreJugador(std::string nombre);
    std::string getNombreJugador();


private:
    std::string nombreJugador;
    bool dijoUno;

};


#endif // JUGADOR_H
