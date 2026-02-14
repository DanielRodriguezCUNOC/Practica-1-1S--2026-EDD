#ifndef JUGADOR_H
#define JUGADOR_H
#include <QString>

class Jugador
{
public:
    Jugador();
    void setNombreJugador(QString nombre);
    QString getNombreJugador();


private:
    QString nombreJugador;


};


#endif // JUGADOR_H
