#include "ladocoloreterno.h"
#include "juego.h"

LadoColorEterno::LadoColorEterno(Color c, int n, std::string ruta):
    LadoCarta(TipoCarta::COLORETERNO, c, n, ruta){}

LadoCarta* LadoColorEterno::clonar() const {
    return new LadoColorEterno(*this);
}

void LadoColorEterno::aplicarEfecto(Juego* juego, const std::string& colorSeleccionado,
                                    const std::string jugadorSeleccionado, int numeroSeleccionado) {

    juego->avanzarTurno();
    Jugador* victima = juego->getJugadorActual();
    Color colorRobado = Color::NEGRO;
    Color colorObjetivo = juego->convertirStringAColor(colorSeleccionado);

    do {
        if(juego->mazoEstaVacio()) juego->barajarDescarte();

        Carta robada = juego->robarDelMazo();

        if(!robada.esValida()) break;

        colorRobado = robada.getLadoActivo()->getColor();
        if(robada.esValida())victima->agregarCarta(robada);

    } while(colorRobado != colorObjetivo);
}
