#ifndef JUGADOR_H
#define JUGADOR_H
#include <string>
#include "carta.h"
#include "listagenerica.h"

class Juego;


class Jugador
{

private:
    std::string nombreJugador;
    bool dijoUno;
    ListaGenerica<Carta*> mano;

public:
    Jugador();
    ~Jugador();
    Jugador(const std::string& nombre);
    void setNombreJugador(const std::string& nombre);
    std::string getNombreJugador()const;


    ListaGenerica<Carta*>& getMano();
    void agregarCarta(Carta* carta);
    void quitarCarta(Carta* carta);
    int cantidadCartas() const;

    Carta* seleccionarCarta(int indice);
    void robarCartas(int cantidad, Juego& juego);
    Carta* usarCarta(int indice);
    void decirUNO();
    bool haDichoUNO() const;
    void resetearUNO();
};


#endif // JUGADOR_H
