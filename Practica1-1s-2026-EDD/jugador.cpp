#include "jugador.h"
#include "juego.h"


Jugador::Jugador() : nombreJugador(""), dijoUno(false) {}

Jugador::Jugador(const std::string& nombre)
    : nombreJugador(nombre), dijoUno(false) {}

void Jugador::setNombreJugador(const std::string& nombre)
{
    nombreJugador = nombre;
}

std::string Jugador::getNombreJugador() const
{
    return nombreJugador;
}

ListaGenerica<Carta*>& Jugador::getMano()
{
    return mano;
}

void Jugador::agregarCarta(Carta* carta)
{
    mano.insertarFinal(carta);
}

void Jugador::quitarCarta(Carta* carta)
{
    mano.eliminarDato(carta);
}

int Jugador::cantidadCartas() const
{
    return mano.getSize();
}

/*void Jugador::robarCartas(int cantidad, Juego& juego)
{
    for (int i = 0; i < cantidad; i++)
    {
        Carta* c = juego.robarDelMazo();
        if (c)
            mano.insertarFinal(c);
    }
}*/

void Jugador::decirUNO()
{
    dijoUno = true;
}

bool Jugador::haDichoUNO() const
{
    return dijoUno;
}

