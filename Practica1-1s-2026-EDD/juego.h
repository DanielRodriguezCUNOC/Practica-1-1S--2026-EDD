#ifndef JUEGO_H
#define JUEGO_H

#include "carta.h"
#include "jugador.h"
#include "listagenerica.h"
#include "rutaimagenes.h"
#include "utils.h"
#include <qwindowdefs.h>
#include <QObject>

class Juego: public QObject
{
    Q_OBJECT
private:
    // ====== ESTRUCTURAS PRINCIPALES ======
    ListaGenerica<Carta> mazo;
    ListaGenerica<Carta> descarte;
    ListaGenerica<Jugador*> jugadores;

    // ====== ESTADO DEL JUEGO ======
    bool ladoOscuroActivo;
    int indiceTurnoActual;
    int sentidoJuego;
    Color colorActivo;

    // ====== UTILIDADES ======
    RutaImagenes ruta;

    // ====== GENERACIÓN Y MEZCLA ======
    void generarMazoNormalManual(int numMazos);
    void generarMazoFlipManual(int numMazos);
    void sacarPrimeraCarta();

    void mezclarArregloLados(LadoCarta* arreglo[], int tamano);
    void mezclarArregloCartas(Carta arreglo[], int tamano);

    int cartasPorPartida(int cantJugadores);

    void jugarCartaSinSeñales(Jugador* jugador, int indiceCartaEnMano,
                              const std::string& jugadorSeleccionado,
                              int numeroAdivinado,
                              const std::string& colorAdivinado);


signals:
    void cartaJugadaSignal(int idCarta);
    void manoActualizadaSignal();
    void turnoCambiadoSignal(QString nombreJugador);
    void mazoActualizadoSignal(int cartasRestantes);
    void descarteActualizadoSignal(QString rutaCarta);
    void partidaIniciadaSignal();
    void cartaInvalidaSignal();

public:
    // ====== CONSTRUCTOR / DESTRUCTOR ======
    Juego(QObject* parent = nullptr);
    ~Juego();

    // ====== INICIALIZACIÓN ======
    void inicializarMazo(int cantidadJugadores, bool modoFlip);
    void repartirCartas(int cartasPorJugador = 7);

    // ====== TURNOS ======
    void avanzarTurno();
    void retrocederTurno();

    // ====== ACCIONES DE JUEGO ======
    void jugarCarta(
        Jugador* jugador,
        int indiceCartaEnMano,
        const std::string& jugadorSeleccionado = "",
        int numeroAdivinado = -1,
        const std::string& colorAdivinado = ""
        );

    void aplicarEfectoCarta(
        Carta cartaJugada,
        const std::string jugadorSeleccionado = "",
        int numeroSeleccionado = -1,
        const std::string& colorSeleccionado = ""
        );

    // ====== MAZO Y DESCARTE ======
    void barajarMazo();
    void barajarDescarte();

    Carta robarDelMazo();
    void agregarADescarte(Carta carta);

    bool mazoEstaVacio() const;

    // ====== CARTAS ======
    bool adivinoCarta(
        const std::string& nombreJugador,
        int numeroCarta,
        const std::string& colorCarta
        );

    Color convertirStringAColor(const std::string& colorStr);

    bool puedeJugarCarta(const Carta& carta);

    // ====== JUGADORES ======
    Jugador* getJugadorActual();
    Jugador* getJugadorSeleccionado(const std::string& nombreJugador);
    Jugador* getJugadorEnPosicion(int indice);

    int getCantidadJugadores() const;

    // ====== GETTERS ======
    int getSentidoJuego() const;
    bool getLadoOscuroActivo() const;
    Color getColorActivo() const;
    int getIndiceTurnoActual()const;

    int getTamanoMazo() const;
    int getTamanoDescarte() const;

    ListaGenerica<Carta>& getMazo();
    ListaGenerica<Carta>& getDescarte();
    ListaGenerica<Jugador*>& getJugadores();

    // ====== SETTERS ======
    void setSentidoJuego(int sentido);
    void setLadoOscuroActivo(bool activo);
    void setColorActivo(Color color);
    void setIndiceTurnoActual(int nuevoIndice);
    void setJugadorEnLista(const std::string nombreJugador);

public slots:
    // Para recibir acciones de la UI
    void onCartaJugadaSlot(int indiceCarta);
};

#endif
