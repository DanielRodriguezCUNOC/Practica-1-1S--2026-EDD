#include "ladoadivinarcarta.h"
#include "juego.h"

LadoAdivinarCarta::LadoAdivinarCarta(Color c, int n, std::string ruta):
    LadoCarta(TipoCarta::ADIVINARCARTA, c, n, ruta){}

LadoCarta* LadoAdivinarCarta::clonar() const {
    return new LadoAdivinarCarta(*this);
}

void LadoAdivinarCarta::aplicarEfecto(Juego* juego, const std::string& colorSeleccionado,
                                      const std::string jugadorSeleccionado, int numeroSeleccionado) {

    //* Verificamos que los datos esten completos
    if (jugadorSeleccionado.empty() || numeroSeleccionado == -1 || colorSeleccionado.empty()) {
        // Error: Faltan datos para adivinar carta
        return;
    }

    Jugador* jugadorActual = juego->getJugadorActual();
    bool adivino = adivinoCarta(juego, jugadorSeleccionado, numeroSeleccionado, colorSeleccionado);

    if (adivino) {
        Color colorObjetivo = juego->convertirStringAColor(colorSeleccionado);
        ListaGenerica<Carta>& manoActual = jugadorActual->getMano();

        // El jugador manda las cartas del mismo color a la pila de descarte
        for(int i = manoActual.getSize() - 1; i >= 0; i--) {
            Carta carta = manoActual.obtenerElementoEnPosicion(i);
            Color colorCarta = carta.getLadoActivo()->getColor();

            if(colorCarta == colorObjetivo) {
                Carta cartaDescartar = manoActual.obtenerElementoEnPosicion(i);
                manoActual.eliminarDatoEnPosicion(i);
                juego->agregarADescarte(cartaDescartar);
            }
        }
    } else {
        // El jugador que intentó adivinar roba dos cartas como penalización
        Jugador* jugadorActual = juego->getJugadorActual();
        for (int i = 0; i < 2; i++) {
            if(juego->mazoEstaVacio()) juego->barajarDescarte();
            Carta robada = juego->robarDelMazo();
            if(robada.esValida())jugadorActual->agregarCarta(robada);
        }
    }
}

bool LadoAdivinarCarta::adivinoCarta(Juego* juego,
    std::string nombreSeleccionado, int numeroCarta,
    std::string colorCarta)const{

    Jugador* jugadorSeleccionado = juego->getJugadorSeleccionado(nombreSeleccionado);

    if (!jugadorSeleccionado) return false;

    // Convertir el string color a enum Color para comparar
    Color colorAdivinado = juego->convertirStringAColor(colorCarta);

    // Recorrer la mano del jugador seleccionado
    ListaGenerica<Carta>& manoSeleccionada = jugadorSeleccionado->getMano();

    for (int i = 0; i < manoSeleccionada.getSize(); i++) {
        Carta carta = manoSeleccionada.obtenerElementoEnPosicion(i);
        const LadoCarta* lado = carta.getLadoActivo();

        // Verificar si coincide número y color
        if (lado->getTipo() == TipoCarta::NUMERO &&
            lado->getNumero() == numeroCarta &&
            lado->getColor() == colorAdivinado) {
            return true;
        }
    }
    return false;
}
