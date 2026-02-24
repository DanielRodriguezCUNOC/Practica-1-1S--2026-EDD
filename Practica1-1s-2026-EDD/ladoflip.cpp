#include "ladoflip.h"
#include "juego.h"
#include <QDebug>
#include <QTimer>

LadoFlip::LadoFlip(Color c, int n, std::string ruta):
    LadoCarta(TipoCarta::FLIP, c, n, ruta){}

LadoCarta* LadoFlip::clonar() const {
    return new LadoFlip(*this);
}

void LadoFlip::aplicarEfecto(Juego* juego, const std::string& colorSeleccionado,
                             const std::string jugadorSeleccionado, int numeroSeleccionado) {

    qDebug() << "[FLIP] =========================";
    qDebug() << "[FLIP] INICIANDO EFECTO FLIP";
    qDebug() << "[FLIP] =========================";

    if (!juego) {
        qDebug() << "[FLIP ERROR] Juego es nullptr";
        return;
    }

    try {
        // Cambiar el estado global primero
        bool nuevoEstado = !juego->getLadoOscuroActivo();
        juego->setLadoOscuroActivo(nuevoEstado);

        qDebug() << "[FLIP] Estado cambiado a:" << (nuevoEstado ? "OSCURO" : "CLARO");

        // Voltear todas las cartas de forma directa (sin QTimer)
        qDebug() << "[FLIP] Volteando cartas del mazo...";
        for(int i = 0; i < juego->getMazo().getSize(); i++) {
            juego->getMazo().obtenerElementoEnPosicion(i).voltear();
        }

        qDebug() << "[FLIP] Volteando cartas del descarte...";
        for(int i = 0; i < juego->getDescarte().getSize(); i++) {
            juego->getDescarte().obtenerElementoEnPosicion(i).voltear();
        }

        qDebug() << "[FLIP] Volteando manos de jugadores...";
        for(int i = 0; i < juego->getCantidadJugadores(); i++) {
            Jugador* jugador = juego->getJugadorEnPosicion(i);
            if (jugador) {
                for(int j = 0; j < jugador->getMano().getSize(); j++) {
                    jugador->getMano().obtenerElementoEnPosicion(j).voltear();
                }
            }
        }

        qDebug() << "[FLIP] Volteado completado, emitiendo señales...";

        // Emitir señales de actualización
        emit juego->manoActualizadaSignal();

        if (juego->getDescarte().getSize() > 0) {
            Carta cartaDescarte = juego->getDescarte().obtenerElementoEnPosicion(0);
            if (cartaDescarte.esValida()) {
                emit juego->descarteActualizadoSignal(
                    QString::fromStdString(cartaDescarte.getLadoActivo()->getRutaArchivo()));
            }
        }

        qDebug() << "[FLIP] EFECTO FLIP COMPLETADO EXITOSAMENTE";

    } catch (...) {
        qDebug() << "[FLIP ERROR] Error crítico en aplicarEfecto";
    }
}
