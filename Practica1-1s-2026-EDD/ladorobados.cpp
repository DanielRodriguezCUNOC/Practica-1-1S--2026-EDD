
#include "ladorobados.h"
#include "juego.h"

LadoRobaDos::LadoRobaDos(Color c, int n, std::string ruta) : LadoCarta(TipoCarta::ROBA2, c, n, ruta) {}

LadoCarta *LadoRobaDos::clonar() const
{
    return new LadoRobaDos(*this);
}
void LadoRobaDos::aplicarEfecto(Juego *juego, const std::string &colorSeleccionado,
                                const std::string jugadorSeleccionado, int numeroSeleccionado)
{
    if (juego->getAcumulacion())
    {
        juego->setPenaAcumulada(juego->getPenaAcumulada() + 2);
        juego->setTipoPenaActual(TipoCarta::ROBA2);
        return;
    }
    juego->avanzarTurno();
    Jugador *victima = juego->getJugadorActual();
    for (int i = 0; i < 2; i++)
    {
        if (juego->mazoEstaVacio())
            juego->barajarDescarte();
        Carta robada = juego->robarDelMazo();
        if (robada.esValida())
            victima->agregarCarta(robada);
    }
}
