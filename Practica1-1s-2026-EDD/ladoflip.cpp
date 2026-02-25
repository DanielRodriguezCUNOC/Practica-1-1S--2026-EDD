#include "ladoflip.h"
#include "juego.h"

LadoFlip::LadoFlip(Color c, int n, std::string ruta) : LadoCarta(TipoCarta::FLIP, c, n, ruta) {}

LadoCarta *LadoFlip::clonar() const
{
    return new LadoFlip(*this);
}

void LadoFlip::aplicarEfecto(Juego *juego, const std::string &colorSeleccionado,
                             const std::string jugadorSeleccionado, int numeroSeleccionado)
{

    if (!juego)
        return;

    try
    {
        bool nuevoEstado = !juego->getLadoOscuroActivo();
        juego->setLadoOscuroActivo(nuevoEstado);

        for (int i = 0; i < juego->getMazo().getSize(); i++)
        {
            juego->getMazo().obtenerElementoEnPosicion(i).voltear();
        }

        for (int i = 0; i < juego->getDescarte().getSize(); i++)
        {
            juego->getDescarte().obtenerElementoEnPosicion(i).voltear();
        }

        for (int i = 0; i < juego->getCantidadJugadores(); i++)
        {
            Jugador *jugador = juego->getJugadorEnPosicion(i);
            if (jugador)
            {
                for (int j = 0; j < jugador->getMano().getSize(); j++)
                {
                    jugador->getMano().obtenerElementoEnPosicion(j).voltear();
                }
            }
        }

        emit juego->manoActualizadaSignal();

        if (juego->getDescarte().getSize() > 0)
        {
            Carta cartaDescarte = juego->getDescarte().obtenerElementoEnPosicion(0);
            if (cartaDescarte.esValida())
            {
                emit juego->descarteActualizadoSignal(
                    QString::fromStdString(cartaDescarte.getLadoActivo()->getRutaArchivo()));
            }
        }
    }
    catch (...)
    {
    }
}
