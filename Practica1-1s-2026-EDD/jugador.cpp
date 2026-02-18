#include "jugador.h"
#include "juego.h"


Jugador::Jugador() : nombreJugador(""), dijoUno(false) {}

Jugador::Jugador(const std::string& nombre)
    : nombreJugador(nombre), dijoUno(false) {}
Jugador::~Jugador(){}

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

void Jugador::resetearUNO(){
    dijoUno = false;
}

void Jugador::robarCartas(int cantidad, Juego& juego)
{
    for (int i = 0; i < cantidad; i++)
    {
        Carta* nueva = juego.robarDelMazo();
        if (nueva != nullptr){
            mano.insertarFinal(nueva);
            resetearUNO();
        }else{
            //* Manejar logica de mazo vacio
            break;
        }

    }
}

Carta* Jugador::seleccionarCarta(int indice){
    if (indice < 0 || indice >= mano.getSize()) return nullptr;
return mano.obtenerElementoEnPosicion(indice);
}

Carta* Jugador::usarCarta(int indice){
    if(indice < 0 || indice >= mano.getSize()) return nullptr;
    Carta* cartaAUsar = seleccionarCarta(indice);
    mano.eliminarDatoEnPosicion(indice);
    return cartaAUsar;
}

void Jugador::decirUNO()
{
    dijoUno = true;
}

bool Jugador::haDichoUNO() const
{
    return dijoUno;
}

