#ifndef JUEGO_H
#define JUEGO_H

#include "carta.h"
#include "jugador.h"
#include "listagenerica.h"
#include "rutaimagenes.h"
#include "utils.h"
#include <qwindowdefs.h>
#include <QObject>

class Juego : public QObject
{
    Q_OBJECT
private:
    // ====== ESTRUCTURAS PRINCIPALES ======
    ListaGenerica<Carta> mazo;
    ListaGenerica<Carta> descarte;
    ListaGenerica<Jugador *> jugadores;

    // ====== ESTADO DEL JUEGO ======
    bool ladoOscuroActivo;
    int indiceTurnoActual;
    int sentidoJuego;
    Color colorActivo;
    bool acumulacionActiva;
    int penaAcumulada;
    TipoCarta tipoPenaActual;
    bool retoMasCuatroActivo;
    bool retoPendiente;
    int indiceJugadorLanzador;
    Color colorAntesDeReto;
    int numAnteDeReto;
    bool robarSinLimite;
    bool gritoUnoActivo;
    bool gritoUnoPendiente;
    int indiceJugadorConUno;
    bool ganarConNegraActivo;

    // ====== UTILIDADES ======
    RutaImagenes ruta;

    // ====== GENERACIÓN Y MEZCLA ======
    void generarMazoNormalManual(int numMazos);
    void generarMazoFlipManual(int numMazos);
    void sacarPrimeraCarta();

    void mezclarArregloLados(LadoCarta *arreglo[], int tamano);
    void mezclarArregloCartas(Carta arreglo[], int tamano);

    int cartasPorPartida(int cantJugadores);

    void jugarCartaSinSeñales(Jugador *jugador, int indiceCartaEnMano,
                              const std::string &jugadorSeleccionado,
                              int numeroAdivinado,
                              const std::string &colorAdivinado);

signals:
    void cartaJugadaSignal(int idCarta);
    void manoActualizadaSignal();
    void turnoCambiadoSignal(QString nombreJugador);
    void mazoActualizadoSignal(int cartasRestantes);
    void descarteActualizadoSignal(QString rutaCarta);
    void partidaIniciadaSignal();
    void cartaInvalidaSignal();
    void debeJugarAntesDeRobarSignal();                      // Jugador intenta robar teniendo carta válida
    void mazoSinCartasSignal();                              // No quedan cartas disponibles
    void pedirColorSignal(int indiceCarta, bool modoOscuro); // Comodín necesita selección de color
    void pedirDatosAdivinarSignal(int indiceCarta);          // AdivinarCarta necesita datos
    void retoPosibleSignal();                                // Víctima del +4 puede retar
    void retoResultadoSignal(bool retoExitoso);              // Resultado del reto
    void unoReportadoSignal(bool fueValido, QString nombreCulpable); // Resultado del reporte UNO
    void cartaNegraBloqueadaSignal();                        // No se puede ganar con carta negra

public:
    // ====== CONSTRUCTOR / DESTRUCTOR ======
    Juego(QObject *parent = nullptr);
    ~Juego();

    // ====== INICIALIZACIÓN ======
    void inicializarMazo(int cantidadJugadores, bool modoFlip);
    void repartirCartas(int cartasPorJugador = 7);

    // ====== TURNOS ======
    void avanzarTurno();
    void retrocederTurno();

    // ====== ACCIONES DE JUEGO ======
    void jugarCarta(
        Jugador *jugador,
        int indiceCartaEnMano,
        const std::string &jugadorSeleccionado = "",
        int numeroAdivinado = -1,
        const std::string &colorAdivinado = "");

    void aplicarEfectoCarta(
        Carta cartaJugada,
        const std::string jugadorSeleccionado = "",
        int numeroSeleccionado = -1,
        const std::string &colorSeleccionado = "");

    // ====== MAZO Y DESCARTE ======
    void barajarMazo();
    void barajarDescarte();

    Carta robarDelMazo();
    void agregarADescarte(Carta carta);

    bool mazoEstaVacio() const;

    // ====== CARTAS ======
    bool adivinoCarta(
        const std::string &nombreJugador,
        int numeroCarta,
        const std::string &colorCarta);

    Color convertirStringAColor(const std::string &colorStr);

    bool puedeJugarCarta(const Carta &carta);

    // ====== JUGADORES ======
    Jugador *getJugadorActual();
    Jugador *getJugadorSeleccionado(const std::string &nombreJugador);
    Jugador *getJugadorEnPosicion(int indice);

    int getCantidadJugadores() const;

    // ====== GETTERS ======
    int getSentidoJuego() const;
    bool getLadoOscuroActivo() const;
    Color getColorActivo() const;
    int getIndiceTurnoActual() const;

    int getTamanoMazo() const;
    int getTamanoDescarte() const;

    ListaGenerica<Carta> &getMazo();
    ListaGenerica<Carta> &getDescarte();
    ListaGenerica<Jugador *> &getJugadores();

    // ====== SETTERS ======
    void setSentidoJuego(int sentido);
    void setLadoOscuroActivo(bool activo);
    void setColorActivo(Color color);
    void setIndiceTurnoActual(int nuevoIndice);
    void setJugadorEnLista(const std::string nombreJugador);
    void setAcumulacion(bool activo);
    void setPenaAcumulada(int pena);
    void setTipoPenaActual(TipoCarta tipo);
    bool getAcumulacion() const;
    int getPenaAcumulada() const;
    TipoCarta getTipoPenaActual() const;
    void setRetoMasCuatro(bool activo);
    bool getRetoMasCuatro() const;
    bool getRetoPendiente() const;
    void resolverReto();
    void setRobarSinLimite(bool activo);
    void setGritoUno(bool activo);
    bool getGritoUnoPendiente() const;
    void setGanarConNegra(bool activo);
    void avisarUno();
    void reportarUno();

    // Continuación de jugadas que requieren input del usuario
    void jugarCartaConColor(int indiceCarta, const std::string &color);
    void jugarCartaAdivinar(int indiceCarta, const std::string &color,
                            const std::string &jugador, int numero);

public slots:
    // Para recibir acciones de la UI
    void onCartaJugadaSlot(int indiceCarta);
    void intentarRobarCarta(); // Valida y ejecuta robo de carta
};

#endif
