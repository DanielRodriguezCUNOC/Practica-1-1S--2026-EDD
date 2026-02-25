#include "juego.h"
#include "ladonumero.h"
#include "ladorobauno.h"
#include "ladorobados.h"
#include "ladorobatres.h"
#include "ladoadivinarcarta.h"
#include "ladocambiardireccion.h"
#include "ladocambiarmano.h"
#include "ladoadivinarcarta.h"
#include "ladocomodincolor.h"
#include "ladocomodinrobardos.h"
#include "ladocomodinrobarcuatro.h"
#include "ladocomodinrobarseis.h"
#include "ladocoloreterno.h"
#include "ladoreverse.h"
#include "ladosalto.h"
#include "ladoflip.h"
#include "ladosaltotodos.h"
#include <cstdlib>
#include <ctime>
#include <QWidget>
#include <QTimer>

Juego::Juego(QObject *parent) : QObject(parent), ladoOscuroActivo(false),
                                indiceTurnoActual(0), sentidoJuego(1),
                                acumulacionActiva(false), penaAcumulada(0),
                                tipoPenaActual(TipoCarta::NUMERO),
                                retoMasCuatroActivo(false), retoPendiente(false),
                                indiceJugadorLanzador(0),
                                colorAntesDeReto(Color::INDEFINIDO), numAnteDeReto(-1),
                                robarSinLimite(false),
                                gritoUnoActivo(false), gritoUnoPendiente(false),
                                indiceJugadorConUno(0),
                                ganarConNegraActivo(false)
{
}

Juego::~Juego()
{

    while (!jugadores.estaVacia())
    {
        Jugador *j = jugadores.obtenerElementoEnPosicion(0);
        delete j;
        jugadores.eliminarDatoEnPosicion(0);
    }
}

void Juego::inicializarMazo(int cantidadJugadores, bool modoFlip)
{
    // Limpiar mazo y descarte anteriores

    while (mazo.getSize() > 0)
    {
        mazo.eliminarDatoEnPosicion(0);
    }
    while (descarte.getSize() > 0)
    {
        descarte.eliminarDatoEnPosicion(0);
    }

    ladoOscuroActivo = false;
    penaAcumulada = 0;
    tipoPenaActual = TipoCarta::NUMERO;
    retoPendiente = false;
    gritoUnoPendiente = false;
    int numMazos = ((cantidadJugadores - 1) / 6) + 1;

    // Crear el mazo según el modo
    if (modoFlip)
    {
        generarMazoFlipManual(numMazos);
    }
    else
    {
        generarMazoNormalManual(numMazos);
    }

    // MEZCLAR EL MAZO COMPLETO
    barajarMazo();

    // Repartir cartas a jugadores
    repartirCartas();

    // Sacar primera carta para descarte
    sacarPrimeraCarta();

    emit partidaIniciadaSignal();
}

void Juego::sacarPrimeraCarta()
{
    if (!mazo.estaVacia())
    {
        Carta primeraCarta = robarDelMazo();
        descarte.insertarInicio(primeraCarta);

        // Sincronizar colorActivo con la primera carta del descarte
        LadoCarta *lado = primeraCarta.getLadoActivo();
        if (lado && lado->getColor() != Color::NEGRO && lado->getColor() != Color::INDEFINIDO)
        {
            colorActivo = lado->getColor();
        }

        emit descarteActualizadoSignal(QString::fromStdString(
            primeraCarta.getLadoActivo()->getRutaArchivo()));
    }
}

void Juego::mezclarArregloLados(LadoCarta *arreglo[], int tamano)
{
    // Algoritmo de Fisher-Yates para barajar el arreglo
    for (int i = tamano - 1; i > 0; i--)
    {
        // Generar un índice aleatorio entre 0 e i
        int j = rand() % (i + 1);

        // Intercambiar arreglo[i] con arreglo[j]
        LadoCarta *temporal = arreglo[i];
        arreglo[i] = arreglo[j];
        arreglo[j] = temporal;
    }
}

void Juego::mezclarArregloCartas(Carta arreglo[], int size)
{
    for (int i = size - 1; i > 0; --i)
    {
        int j = rand() % (i + 1);

        Carta temp = arreglo[i];
        arreglo[i] = arreglo[j];
        arreglo[j] = temp;
    }
}

void Juego::generarMazoNormalManual(int numMazos)
{
    for (int n = 0; n < numMazos; n++)
    {
        Color colores[] = {Color::ROJO, Color::AZUL, Color::VERDE, Color::AMARILLO};

        // === CARTAS NUMÉRICAS ===
        // 1 carta de 0 por color (4 cartas)
        for (Color color : colores)
        {
            std::string rutaNumero0 = ruta.generarRuta(TipoCarta::NUMERO, color, 0);
            mazo.insertarFinal(Carta(new LadoNumero(color, 0, rutaNumero0)));
        }

        // 2 cartas de cada número 1-9 por color (72 cartas)
        for (Color color : colores)
        {
            for (int numero = 1; numero <= 9; numero++)
            {
                std::string rutaNumero = ruta.generarRuta(TipoCarta::NUMERO, color, numero);
                // Primera carta
                mazo.insertarFinal(Carta(new LadoNumero(color, numero, rutaNumero)));
                // Segunda carta
                mazo.insertarFinal(Carta(new LadoNumero(color, numero, rutaNumero)));
            }
        }

        // === CARTAS ESPECIALES DE COLOR ===
        // 2 cartas de cada tipo por color
        for (Color color : colores)
        {
            // 2 cartas +2
            for (int i = 0; i < 2; i++)
            {
                std::string rutaRoba2 = ruta.generarRuta(TipoCarta::ROBA2, color);
                mazo.insertarFinal(Carta(new LadoRobaDos(color, -1, rutaRoba2)));
            }

            // 2 cartas Bloqueo
            for (int i = 0; i < 2; i++)
            {
                std::string rutaBloqueo = ruta.generarRuta(TipoCarta::BLOQUEO, color);
                mazo.insertarFinal(Carta(new LadoSalto(color, -1, rutaBloqueo)));
            }

            // 2 cartas Reversa
            for (int i = 0; i < 2; i++)
            {
                std::string rutaReverse = ruta.generarRuta(TipoCarta::REVERSE, color);
                mazo.insertarFinal(Carta(new LadoReverse(color, -1, rutaReverse)));
            }
        }

        // === CARTAS COMODÍN ===
        // 4 cartas de cada tipo comodín
        std::string rutaComodinColor = ruta.generarRuta(TipoCarta::COMODIN, Color::NEGRO);
        std::string rutaComodinRoba4 = ruta.generarRuta(TipoCarta::COMODIN4, Color::NEGRO);
        std::string rutaComodinAdivinar = ruta.generarRuta(TipoCarta::ADIVINARCARTA, Color::NEGRO);
        std::string rutaComodinCambiarMano = ruta.generarRuta(TipoCarta::CAMBIARMANO, Color::NEGRO);

        for (int i = 0; i < 4; i++)
        {
            mazo.insertarFinal(Carta(new LadoComodinColor(Color::NEGRO, -1, rutaComodinColor)));
            mazo.insertarFinal(Carta(new LadoComodinRobarCuatro(Color::NEGRO, -1, rutaComodinRoba4)));
            mazo.insertarFinal(Carta(new LadoAdivinarCarta(Color::NEGRO, -1, rutaComodinAdivinar)));
            mazo.insertarFinal(Carta(new LadoCambiarMano(Color::NEGRO, -1, rutaComodinCambiarMano)));
        }
    }
}

void Juego::generarMazoFlipManual(int numMazos)
{
    for (int n = 0; n < numMazos; n++)
    {
        Color cClaros[] = {Color::ROJO, Color::AZUL, Color::VERDE, Color::AMARILLO};
        Color cOscuros[] = {Color::ROSA, Color::TURQUESA, Color::NARANJA, Color::PURPURA};

        // === CARTAS NUMÉRICAS === (76 cartas: 38 cartas x 2 lados = 76 efectos)

        // 1 carta de 0 por cada color (4 cartas)
        for (int i = 0; i < 4; i++)
        {
            LadoCarta *ladoClaro = new LadoNumero(cClaros[i], 0, ruta.generarRuta(TipoCarta::NUMERO, cClaros[i], 0));
            LadoCarta *ladoOscuro = new LadoNumero(cOscuros[i], 0, ruta.generarRuta(TipoCarta::NUMERO, cOscuros[i], 0));
            mazo.insertarFinal(Carta(ladoClaro, ladoOscuro));
        }

        // 2 cartas de cada número 1-9 por color (36 cartas)
        for (int color = 0; color < 4; color++)
        {
            for (int numero = 1; numero <= 9; numero++)
            {
                // Primera carta del número
                LadoCarta *ladoClaro1 = new LadoNumero(cClaros[color], numero, ruta.generarRuta(TipoCarta::NUMERO, cClaros[color], numero));
                LadoCarta *ladoOscuro1 = new LadoNumero(cOscuros[color], numero, ruta.generarRuta(TipoCarta::NUMERO, cOscuros[color], numero));
                mazo.insertarFinal(Carta(ladoClaro1, ladoOscuro1));

                // Segunda carta del número
                LadoCarta *ladoClaro2 = new LadoNumero(cClaros[color], numero, ruta.generarRuta(TipoCarta::NUMERO, cClaros[color], numero));
                LadoCarta *ladoOscuro2 = new LadoNumero(cOscuros[color], numero, ruta.generarRuta(TipoCarta::NUMERO, cOscuros[color], numero));
                mazo.insertarFinal(Carta(ladoClaro2, ladoOscuro2));
            }
        }
        // Total numéricas: 76 cartas

        // === CARTAS ESPECIALES DE COLOR === (32 cartas)

        for (int color = 0; color < 4; color++)
        {
            // 2 cartas: +1 (claro) / +3 (oscuro)
            for (int i = 0; i < 2; i++)
            {
                LadoCarta *ladoRoba1 = new LadoRobaUno(cClaros[color], -1, ruta.generarRuta(TipoCarta::ROBA1, cClaros[color]));
                LadoCarta *ladoRoba3 = new LadoRobaTres(cOscuros[color], -1, ruta.generarRuta(TipoCarta::ROBA3, cOscuros[color]));
                mazo.insertarFinal(Carta(ladoRoba1, ladoRoba3));
            }

            // 2 cartas: Cambio dirección (ambos lados)
            for (int i = 0; i < 2; i++)
            {
                LadoCarta *ladoCambioClaro = new LadoCambiarDireccion(cClaros[color], -1, ruta.generarRuta(TipoCarta::CAMBIARDIRECCION, cClaros[color]));
                LadoCarta *ladoCambioOscuro = new LadoCambiarDireccion(cOscuros[color], -1, ruta.generarRuta(TipoCarta::CAMBIARDIRECCION, cOscuros[color]));
                mazo.insertarFinal(Carta(ladoCambioClaro, ladoCambioOscuro));
            }

            // 2 cartas: Bloqueo (claro) / Salto todos (oscuro)
            for (int i = 0; i < 2; i++)
            {
                LadoCarta *ladoBloqueo = new LadoSalto(cClaros[color], -1, ruta.generarRuta(TipoCarta::BLOQUEO, cClaros[color]));
                LadoCarta *ladoSaltoTodos = new LadoSaltoTodos(cOscuros[color], -1, ruta.generarRuta(TipoCarta::SALTODOS, cOscuros[color]));
                mazo.insertarFinal(Carta(ladoBloqueo, ladoSaltoTodos));
            }

            // 2 cartas: FLIP (ambos lados)
            for (int i = 0; i < 2; i++)
            {
                LadoCarta *ladoFlipClaro = new LadoFlip(cClaros[color], -1, ruta.generarRuta(TipoCarta::FLIP, cClaros[color]));
                LadoCarta *ladoFlipOscuro = new LadoFlip(cOscuros[color], -1, ruta.generarRuta(TipoCarta::FLIP, cOscuros[color]));
                mazo.insertarFinal(Carta(ladoFlipClaro, ladoFlipOscuro));
            }
        }

        // === CARTAS COMODÍN === (24 cartas)

        // 4 cartas: +2 (claro) / +6 (oscuro)
        for (int i = 0; i < 4; i++)
        {
            LadoCarta *ladoComodin2 = new LadoComodinRobarDos(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::COMODIN2, Color::NEGRO));
            LadoCarta *ladoComodin6 = new LadoComodinRobarSeis(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::COMODIN6, Color::NEGRO));
            mazo.insertarFinal(Carta(ladoComodin2, ladoComodin6));
        }

        // 4 cartas: Cambio color (claro) / Color eterno (oscuro)
        for (int i = 0; i < 4; i++)
        {
            LadoCarta *ladoComodinColor = new LadoComodinColor(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::COMODIN, Color::NEGRO));
            LadoCarta *ladoColorEterno = new LadoColorEterno(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::COLORETERNO, Color::NEGRO));
            mazo.insertarFinal(Carta(ladoComodinColor, ladoColorEterno));
        }

        // 4 cartas: Adivinar carta (ambos lados)
        for (int i = 0; i < 4; i++)
        {
            LadoCarta *ladoAdivinarClaro = new LadoAdivinarCarta(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::ADIVINARCARTA, Color::NEGRO));
            LadoCarta *ladoAdivinarOscuro = new LadoAdivinarCarta(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::ADIVINARCARTA, Color::NEGRO));
            mazo.insertarFinal(Carta(ladoAdivinarClaro, ladoAdivinarOscuro));
        }

        // 4 cartas: Cambiar mano (ambos lados)
        for (int i = 0; i < 4; i++)
        {
            LadoCarta *ladoCambiarClaro = new LadoCambiarMano(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::CAMBIARMANO, Color::NEGRO));
            LadoCarta *ladoCambiarOscuro = new LadoCambiarMano(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::CAMBIARMANO, Color::NEGRO));
            mazo.insertarFinal(Carta(ladoCambiarClaro, ladoCambiarOscuro));
        }

        // 4 cartas: Eclipse (claro) / Espía (oscuro)
        for (int i = 0; i < 4; i++)
        {
            LadoCarta *ladoAdivinarCarta = new LadoAdivinarCarta(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::ADIVINARCARTA, Color::NEGRO));
            LadoCarta *ladoCambiarMano = new LadoCambiarMano(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::CAMBIARMANO, Color::NEGRO));
            mazo.insertarFinal(Carta(ladoAdivinarCarta, ladoCambiarMano));
        }

        // 4 cartas: +4 (claro) - estas parecen ser especiales adicionales
        for (int i = 0; i < 4; i++)
        {
            LadoCarta *ladoComodin4Claro = new LadoComodinRobarCuatro(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::COMODIN4, Color::NEGRO));
            LadoCarta *ladoComodin4Oscuro = new LadoComodinRobarCuatro(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::COMODIN4, Color::NEGRO));
            mazo.insertarFinal(Carta(ladoComodin4Claro, ladoComodin4Oscuro));
        }
    }
}
void Juego::avanzarTurno()
{

    // Avanzamos el turno según el sentido (1 o -1)
    indiceTurnoActual += sentidoJuego;

    int numJugadores = jugadores.getSize();

    // si salimos de los límites aplicado a una lista circular
    if (indiceTurnoActual >= numJugadores)
    {
        indiceTurnoActual = 0;
    }
    else if (indiceTurnoActual < 0)
    {
        indiceTurnoActual = numJugadores - 1;
    }
}

Carta Juego::robarDelMazo()
{
    if (mazo.estaVacia())
    {
        return nullptr;
    }
    return mazo.robarCarta();
}

bool Juego::getLadoOscuroActivo() const
{
    return ladoOscuroActivo;
}

void Juego::setLadoOscuroActivo(bool estado)
{
    ladoOscuroActivo = estado;
}

int Juego::getTamanoMazo() const
{
    return mazo.getSize();
}

ListaGenerica<Jugador *> &Juego::getJugadores()
{
    return jugadores;
}

ListaGenerica<Carta> &Juego::getMazo()
{
    return mazo;
}

ListaGenerica<Carta> &Juego::getDescarte()
{
    return descarte;
}

int Juego::getTamanoDescarte() const
{
    return descarte.getSize();
}

Jugador *Juego::getJugadorEnPosicion(int indice)
{
    return jugadores.obtenerElementoEnPosicion(indice);
}

int Juego::getCantidadJugadores() const
{
    return jugadores.getSize();
}

int Juego::getSentidoJuego() const
{
    return sentidoJuego;
}

void Juego::setSentidoJuego(int sentido)
{
    sentidoJuego = sentido;
}

void Juego::agregarADescarte(Carta carta)
{
    descarte.insertarInicio(carta);
}

//* Agregar la UI para refrescarla cada vez que se realiza una accion
void Juego::jugarCarta(Jugador *jugador, int indiceCartaEnMano,
                       const std::string &jugadorSeleccionado,
                       int numeroAdivinado,
                       const std::string &colorAdivinado)
{
    // Verificaciones de seguridad
    if (!jugador)
    {
        return;
    }

    if (indiceCartaEnMano < 0 || indiceCartaEnMano >= jugador->getMano().getSize())
    {
        return;
    }

    try
    {
        // Obtener la carta de forma segura
        Carta cartaJugada = jugador->getMano().obtenerElementoEnPosicion(indiceCartaEnMano);
        if (!cartaJugada.esValida())
        {
            return;
        }

        // El jugador tira la carta (la quitamos de su mano)
        jugador->getMano().eliminarDatoEnPosicion(indiceCartaEnMano);

        // La ponemos en la pila de descarte
        descarte.insertarInicio(cartaJugada);

        // EMITIR SEÑALES: La carta fue jugada
        emit cartaJugadaSignal(indiceCartaEnMano);
        emit manoActualizadaSignal();

        // Actualizar información del descarte
        LadoCarta *ladoActivo = cartaJugada.getLadoActivo();
        if (ladoActivo)
        {
            colorActivo = ladoActivo->getColor();
            QString ruta = QString::fromStdString(ladoActivo->getRutaArchivo());
            emit descarteActualizadoSignal(ruta);
        }

        // Procesamos qué hace esa carta
        aplicarEfectoCarta(cartaJugada, jugadorSeleccionado, numeroAdivinado, colorAdivinado);

        // Preparamos el turno del siguiente jugador
        avanzarTurno();

        // EMITIR SEÑAL DE TURNO CAMBIADO
        Jugador *nuevoJugador = getJugadorActual();
        if (nuevoJugador)
        {
            emit turnoCambiadoSignal(QString::fromStdString(nuevoJugador->getNombreJugador()));
            emit manoActualizadaSignal(); // La mano del nuevo jugador
        }

        // Actualizar estado del mazo
        emit mazoActualizadoSignal(mazo.getSize());
    }
    catch (const std::exception &e)
    {
    }
    catch (...)
    {
    }
}

void Juego::repartirCartas(int cartasPorJugador)
{

    // Verificar que hay jugadores
    if (jugadores.estaVacia())
    {
        return;
    }

    // Verificar que hay suficientes cartas en el mazo
    int cartasNecesarias = jugadores.getSize() * cartasPorJugador;
    if (mazo.getSize() < cartasNecesarias)
    {
        return;
    }

    // Repartir cartas a cada jugador
    for (int i = 0; i < jugadores.getSize(); i++)
    {
        Jugador *jugadorActual = jugadores.obtenerElementoEnPosicion(i);

        for (int j = 0; j < cartasPorJugador; j++)
        {
            Carta cartaARobar = robarDelMazo();
            if (cartaARobar.esValida())
                jugadorActual->agregarCarta(cartaARobar);
        }
    }

    // Colocar la primera carta en el descarte para comenzar el juego
    if (!mazo.estaVacia())
    {
        Carta primeraCarta = robarDelMazo();
        descarte.insertarInicio(primeraCarta);

        // Actualizar el color activo según la primera carta
        if (primeraCarta.esValida())
        {
            colorActivo = primeraCarta.getLadoActivo()->getColor();

            // Si es comodín se elige un color
            if (colorActivo == Color::NEGRO)
            {
                // Elegir un color aleatorio
                Color coloresNormales[] = {Color::ROJO, Color::AZUL,
                                           Color::VERDE, Color::AMARILLO};
                colorActivo = coloresNormales[rand() % 4];
            }
        }
    }
}

Jugador *Juego::getJugadorActual()
{
    return jugadores.obtenerElementoEnPosicion(indiceTurnoActual);
}

Jugador *Juego::getJugadorSeleccionado(const std::string &nombreJugador)
{
    for (int i = 0; i < jugadores.getSize(); i++)
    {
        Jugador *temp = jugadores.obtenerElementoEnPosicion(i);
        if (temp->getNombreJugador() == nombreJugador)
            return temp;
    }
    return nullptr;
}

Color Juego::convertirStringAColor(const std::string &colorStr)
{
    if (colorStr == "Rojo")
        return Color::ROJO;
    if (colorStr == "Azul")
        return Color::AZUL;
    if (colorStr == "Verde")
        return Color::VERDE;
    if (colorStr == "Amarillo")
        return Color::AMARILLO;
    // Si estás en modo oscuro
    if (colorStr == "Rosa")
        return Color::ROSA;
    if (colorStr == "Turquesa")
        return Color::TURQUESA;
    if (colorStr == "Naranja")
        return Color::NARANJA;
    if (colorStr == "Purpura")
        return Color::PURPURA;
    // Valor por defecto
    return Color::NEGRO;
}

void Juego::barajarMazo()
{
    mazo.barajar();
}

void Juego::barajarDescarte()
{
    while (descarte.getSize() > 1)
    {
        Carta carta = descarte.robarCarta();
        mazo.insertarFinal(carta);
    }
    barajarMazo();
}

Color Juego::getColorActivo() const
{
    return colorActivo;
}

void Juego::setColorActivo(Color nuevoColor)
{
    this->colorActivo = nuevoColor;
}

void Juego::setAcumulacion(bool activo) { acumulacionActiva = activo; }
bool Juego::getAcumulacion() const { return acumulacionActiva; }
int Juego::getPenaAcumulada() const { return penaAcumulada; }
void Juego::setPenaAcumulada(int pena) { penaAcumulada = pena; }
TipoCarta Juego::getTipoPenaActual() const { return tipoPenaActual; }
void Juego::setTipoPenaActual(TipoCarta tipo) { tipoPenaActual = tipo; }
void Juego::setRetoMasCuatro(bool activo) { retoMasCuatroActivo = activo; }
bool Juego::getRetoMasCuatro() const { return retoMasCuatroActivo; }
bool Juego::getRetoPendiente() const { return retoPendiente; }
void Juego::setRobarSinLimite(bool activo) { robarSinLimite = activo; }
void Juego::setGritoUno(bool activo) { gritoUnoActivo = activo; }
bool Juego::getGritoUnoPendiente() const { return gritoUnoPendiente; }
void Juego::setGanarConNegra(bool activo) { ganarConNegraActivo = activo; }

void Juego::avisarUno()
{
    if (!gritoUnoActivo)
        return;
    gritoUnoPendiente = false;
}

void Juego::reportarUno()
{
    if (!gritoUnoActivo)
        return;
    Jugador *jugadorActual = getJugadorActual();
    if (!jugadorActual)
        return;

    if (gritoUnoPendiente)
    {
        // Válido: el jugador con 1 carta no avisó UNO → roba 2
        Jugador *culpable = getJugadorEnPosicion(indiceJugadorConUno);
        QString nombreCulpable = culpable ? QString::fromStdString(culpable->getNombreJugador()) : "";
        if (culpable)
        {
            for (int i = 0; i < 2; i++)
            {
                if (mazoEstaVacio())
                    barajarDescarte();
                Carta robada = robarDelMazo();
                if (robada.esValida())
                    culpable->agregarCarta(robada);
            }
        }
        gritoUnoPendiente = false;
        QTimer::singleShot(50, this, [this, nombreCulpable]()
                           {
            emit manoActualizadaSignal();
            emit unoReportadoSignal(true, nombreCulpable); });
    }
    else
    {
        // Reporte erróneo: reportador roba 2
        QString nombreReportador = QString::fromStdString(jugadorActual->getNombreJugador());
        for (int i = 0; i < 2; i++)
        {
            if (mazoEstaVacio())
                barajarDescarte();
            Carta robada = robarDelMazo();
            if (robada.esValida())
                jugadorActual->agregarCarta(robada);
        }
        QTimer::singleShot(50, this, [this, nombreReportador]()
                           {
            emit manoActualizadaSignal();
            emit unoReportadoSignal(false, nombreReportador); });
    }
}

bool Juego::mazoEstaVacio() const
{
    return mazo.getSize() <= 0;
}

void Juego::setJugadorEnLista(const std::string nombreJugador)
{
    Jugador *jugador = new Jugador(nombreJugador);
    jugadores.insertarFinal(jugador);
}

void Juego::onCartaJugadaSlot(int indiceCarta)
{
    // Bloquear múltiples llamadas simultáneas
    static bool procesando = false;
    if (procesando)
    {
        return;
    }

    procesando = true;

    try
    {
        // Cerrar ventana de reporte UNO: el nuevo jugador empieza a actuar
        gritoUnoPendiente = false;

        // Si hay reto pendiente el jugador no puede jugar cartas normales
        if (retoPendiente)
        {
            emit cartaInvalidaSignal();
            QTimer::singleShot(0, this, [this]()
                               { emit manoActualizadaSignal(); });
            procesando = false;
            return;
        }

        // Validaciones básicas
        Jugador *jugadorActual = getJugadorActual();
        if (!jugadorActual || indiceCarta < 0 || indiceCarta >= jugadorActual->getMano().getSize())
        {
            // Reconectar señales de la UI aunque no se haya jugado
            QTimer::singleShot(0, this, [this]()
                               { emit manoActualizadaSignal(); });
            procesando = false;
            return;
        }

        Carta cartaIntentada = jugadorActual->getMano().obtenerElementoEnPosicion(indiceCarta);
        if (!puedeJugarCarta(cartaIntentada))
        {
            emit cartaInvalidaSignal();
            QTimer::singleShot(0, this, [this]()
                               { emit manoActualizadaSignal(); });
            procesando = false;
            return;
        }

        // Ganar con negra: bloquear si es la única carta y es negra (a menos que esté habilitado)
        if (!ganarConNegraActivo &&
            jugadorActual->getMano().getSize() == 1 &&
            cartaIntentada.getLadoActivo() &&
            cartaIntentada.getLadoActivo()->getColor() == Color::NEGRO)
        {
            emit cartaNegraBloqueadaSignal();
            QTimer::singleShot(0, this, [this]()
                               { emit manoActualizadaSignal(); });
            procesando = false;
            return;
        }

        // Determinar si la carta requiere entrada del usuario antes de jugarse
        LadoCarta *ladoActivoCarta = cartaIntentada.getLadoActivo();
        TipoCarta tipoCarta = ladoActivoCarta->getTipo();

        bool necesitaColor = (tipoCarta == TipoCarta::COMODIN ||
                              tipoCarta == TipoCarta::COMODIN4 ||
                              tipoCarta == TipoCarta::COMODIN6 ||
                              tipoCarta == TipoCarta::COMODIN2 ||
                              tipoCarta == TipoCarta::CAMBIARMANO);
        bool necesitaAdivinar = (tipoCarta == TipoCarta::ADIVINARCARTA);

        if (necesitaColor)
        {
            emit pedirColorSignal(indiceCarta, ladoOscuroActivo);
            procesando = false;
            return;
        }

        if (necesitaAdivinar)
        {
            emit pedirDatosAdivinarSignal(indiceCarta);
            procesando = false;
            return;
        }

        // Carta normal: jugar directamente
        jugarCartaSinSeñales(jugadorActual, indiceCarta, "", -1, "");

        QTimer::singleShot(50, this, [this]()
                           {
               emit manoActualizadaSignal();
               Jugador* nuevoJugador = getJugadorActual();
               if (nuevoJugador) {
                   emit turnoCambiadoSignal(QString::fromStdString(nuevoJugador->getNombreJugador()));
               }
               emit mazoActualizadoSignal(mazo.getSize()); });
    }
    catch (...)
    {
    }

    procesando = false;
}

void Juego::jugarCartaConColor(int indiceCarta, const std::string &color)
{
    Jugador *jugadorActual = getJugadorActual();
    if (!jugadorActual)
        return;

    jugarCartaSinSeñales(jugadorActual, indiceCarta, "", -1, color);

    QTimer::singleShot(50, this, [this]()
                       {
        emit manoActualizadaSignal();
        Jugador* nuevoJugador = getJugadorActual();
        if (nuevoJugador)
            emit turnoCambiadoSignal(QString::fromStdString(nuevoJugador->getNombreJugador()));
        emit mazoActualizadoSignal(mazo.getSize());
        if (retoPendiente)
            emit retoPosibleSignal(); });
}

void Juego::jugarCartaAdivinar(int indiceCarta, const std::string &color,
                               const std::string &jugador, int numero)
{
    Jugador *jugadorActual = getJugadorActual();
    if (!jugadorActual)
        return;

    jugarCartaSinSeñales(jugadorActual, indiceCarta, jugador, numero, color);

    QTimer::singleShot(50, this, [this]()
                       {
        emit manoActualizadaSignal();
        Jugador* nuevoJugador = getJugadorActual();
        if (nuevoJugador)
            emit turnoCambiadoSignal(QString::fromStdString(nuevoJugador->getNombreJugador()));
        emit mazoActualizadoSignal(mazo.getSize()); });
}

void Juego::intentarRobarCarta()
{
    Jugador *jugadorActual = getJugadorActual();
    if (!jugadorActual)
        return;

    // Acumulación: si hay pena pendiente el jugador acepta robar todas las cartas acumuladas
    if (acumulacionActiva && penaAcumulada > 0)
    {
        int totalRobar = penaAcumulada;
        penaAcumulada = 0;
        for (int i = 0; i < totalRobar; i++)
        {
            if (mazoEstaVacio())
                barajarDescarte();
            Carta robada = robarDelMazo();
            if (robada.esValida())
                jugadorActual->agregarCarta(robada);
        }
        avanzarTurno();
        emit manoActualizadaSignal();
        Jugador *nuevoJugador = getJugadorActual();
        if (nuevoJugador)
            emit turnoCambiadoSignal(QString::fromStdString(nuevoJugador->getNombreJugador()));
        return;
    }

    // Reto pendiente: víctima acepta las 4 cartas sin retar
    if (retoMasCuatroActivo && retoPendiente)
    {
        retoPendiente = false;
        for (int i = 0; i < 4; i++)
        {
            if (mazoEstaVacio())
                barajarDescarte();
            Carta robada = robarDelMazo();
            if (robada.esValida())
                jugadorActual->agregarCarta(robada);
        }
        avanzarTurno();
        emit manoActualizadaSignal();
        Jugador *nuevoJugador = getJugadorActual();
        if (nuevoJugador)
            emit turnoCambiadoSignal(QString::fromStdString(nuevoJugador->getNombreJugador()));
        return;
    }

    // Cerrar ventana de reporte UNO: el jugador empieza a actuar
    gritoUnoPendiente = false;

    // Responsabilidad de Juego: verificar si el jugador puede jugar antes de robar
    bool tieneCartaValida = false;
    ListaGenerica<Carta> &mano = jugadorActual->getMano();
    for (int i = 0; i < mano.getSize(); i++)
    {
        if (puedeJugarCarta(mano.obtenerElementoEnPosicion(i)))
        {
            tieneCartaValida = true;
            break;
        }
    }

    if (tieneCartaValida)
    {
        emit debeJugarAntesDeRobarSignal();
        return;
    }

    if (robarSinLimite)
    {
        // Opción B: robar hasta encontrar una carta jugable
        bool encontroJugable = false;
        while (!encontroJugable)
        {
            if (mazoEstaVacio())
                barajarDescarte();
            Carta nuevaCarta = robarDelMazo();
            if (!nuevaCarta.esValida())
            {
                emit mazoSinCartasSignal();
                emit manoActualizadaSignal();
                return;
            }
            jugadorActual->agregarCarta(nuevaCarta);
            if (puedeJugarCarta(nuevaCarta))
                encontroJugable = true;
        }
        // No avanzar turno: el jugador debe jugar la carta que encontró
        emit manoActualizadaSignal();
        return;
    }

    // Opción A: robar 1 carta y pasar turno
    if (mazoEstaVacio())
    {
        barajarDescarte();
    }

    Carta nuevaCarta = robarDelMazo();
    if (!nuevaCarta.esValida())
    {
        emit mazoSinCartasSignal();
        return;
    }

    jugadorActual->agregarCarta(nuevaCarta);

    avanzarTurno();

    emit manoActualizadaSignal();
    Jugador *nuevoJugador = getJugadorActual();
    if (nuevoJugador)
    {
        emit turnoCambiadoSignal(QString::fromStdString(nuevoJugador->getNombreJugador()));
    }
}

void Juego::resolverReto()
{
    if (!retoPendiente)
        return;
    retoPendiente = false;

    Jugador *lanzador = getJugadorEnPosicion(indiceJugadorLanzador);
    Jugador *victima = getJugadorActual();
    if (!lanzador || !victima)
        return;

    // Verificar si lanzador tenía carta del color activo anterior o del número anterior
    bool teniaCarta = false;
    ListaGenerica<Carta> &mano = lanzador->getMano();
    for (int i = 0; i < mano.getSize() && !teniaCarta; i++)
    {
        Carta c = mano.obtenerElementoEnPosicion(i);
        if (!c.esValida())
            continue;
        LadoCarta *lado = c.getLadoActivo();
        if (!lado)
            continue;
        if (lado->getColor() == colorAntesDeReto)
            teniaCarta = true;
        else if (lado->getNumero() >= 0 && lado->getNumero() == numAnteDeReto)
            teniaCarta = true;
    }

    if (teniaCarta)
    {
        // Reto exitoso: lanzador roba 4, víctima conserva su turno
        for (int i = 0; i < 4; i++)
        {
            if (mazoEstaVacio())
                barajarDescarte();
            Carta robada = robarDelMazo();
            if (robada.esValida())
                lanzador->agregarCarta(robada);
        }
    }
    else
    {
        // Reto fallido: víctima roba 6, pierde turno
        for (int i = 0; i < 6; i++)
        {
            if (mazoEstaVacio())
                barajarDescarte();
            Carta robada = robarDelMazo();
            if (robada.esValida())
                victima->agregarCarta(robada);
        }
        avanzarTurno();
    }

    bool retoExitoso = teniaCarta;
    QTimer::singleShot(50, this, [this, retoExitoso]()
                       {
        emit manoActualizadaSignal();
        Jugador *nuevoJugador = getJugadorActual();
        if (nuevoJugador)
            emit turnoCambiadoSignal(QString::fromStdString(nuevoJugador->getNombreJugador()));
        emit mazoActualizadoSignal(mazo.getSize());
        emit retoResultadoSignal(retoExitoso); });
}

void Juego::aplicarEfectoCarta(Carta cartaJugada, const std::string jugadorSeleccionado,
                               int numeroSeleccionado, const std::string &colorSeleccionado)
{
    if (!cartaJugada.esValida())
    {
        return;
    }

    LadoCarta *ladoActual = cartaJugada.getLadoActivo();
    if (!ladoActual)
    {
        return;
    }

    try
    {
        // Llamar al método aplicarEfecto del lado activo de la carta
        ladoActual->aplicarEfecto(this, colorSeleccionado, jugadorSeleccionado, numeroSeleccionado);
    }
    catch (...)
    {
    }
}

void Juego::jugarCartaSinSeñales(Jugador *jugador, int indiceCartaEnMano,
                                 const std::string &jugadorSeleccionado,
                                 int numeroAdivinado,
                                 const std::string &colorAdivinado)
{
    if (!jugador || indiceCartaEnMano < 0 || indiceCartaEnMano >= jugador->getMano().getSize())
    {
        return;
    }

    try
    {
        // Guardar estado previo para posible reto del +4
        Color colorPrevio = colorActivo;
        int numPrevio = -1;
        if (!descarte.estaVacia())
        {
            Carta topAntes = descarte.obtenerElementoEnPosicion(0);
            if (topAntes.esValida() && topAntes.getLadoActivo())
                numPrevio = topAntes.getLadoActivo()->getNumero();
        }
        int lanzadorIdx = indiceTurnoActual;

        // Obtener y remover carta
        Carta cartaJugada = jugador->getMano().obtenerElementoEnPosicion(indiceCartaEnMano);
        jugador->getMano().eliminarDatoEnPosicion(indiceCartaEnMano);

        // Agregar al descarte
        descarte.insertarInicio(cartaJugada);

        // Actualizar color activo: si el jugador eligió un color (comodíes/cartas especiales)
        // ese tiene prioridad sobre el color físico de la carta
        LadoCarta *ladoActivo = cartaJugada.getLadoActivo();
        if (ladoActivo)
        {
            colorActivo = ladoActivo->getColor();

            if (!colorAdivinado.empty())
            {
                colorActivo = convertirStringAColor(colorAdivinado);
            }

            QString rutaImagen = QString::fromStdString(ladoActivo->getRutaArchivo());
            emit descarteActualizadoSignal(rutaImagen);
        }

        // Aplicar efecto, No se emiten señales porque sino truena XD
        aplicarEfectoCarta(cartaJugada, jugadorSeleccionado, numeroAdivinado, colorAdivinado);

        if (colorAdivinado.empty() && !descarte.estaVacia())
        {
            Carta topDescarte = descarte.obtenerElementoEnPosicion(0);
            if (topDescarte.esValida())
            {
                LadoCarta *ladoTop = topDescarte.getLadoActivo();
                if (ladoTop)
                {
                    Color colorTop = ladoTop->getColor();
                    if (colorTop != Color::NEGRO && colorTop != Color::INDEFINIDO)
                    {
                        colorActivo = colorTop;
                    }
                }
            }
        }

        // Detectar COMODIN4 con reto habilitado
        if (retoMasCuatroActivo && ladoActivo && ladoActivo->getTipo() == TipoCarta::COMODIN4)
        {
            retoPendiente = true;
            indiceJugadorLanzador = lanzadorIdx;
            colorAntesDeReto = colorPrevio;
            numAnteDeReto = numPrevio;
        }

        // Grito UNO: si el jugador se queda con exactamente 1 carta, debe avisar
        if (gritoUnoActivo && jugador->getMano().getSize() == 1)
        {
            gritoUnoPendiente = true;
            indiceJugadorConUno = lanzadorIdx;
        }
        else
        {
            gritoUnoPendiente = false;
        }

        // Avanzar turno
        avanzarTurno();
    }
    catch (...)
    {
    }
}

bool Juego::puedeJugarCarta(const Carta &carta)
{
    if (!carta.esValida())
    {
        return false;
    }

    if (descarte.estaVacia())
    {
        // Primera carta siempre se puede jugar
        return true;
    }

    try
    {
        Carta cartaEnDescarte = descarte.obtenerElementoEnPosicion(0);
        if (!cartaEnDescarte.esValida())
        {
            return true;
        }

        LadoCarta *ladoCartaJugar = carta.getLadoActivo();
        LadoCarta *ladoDescarte = cartaEnDescarte.getLadoActivo();

        if (!ladoCartaJugar || !ladoDescarte)
        {
            return false;
        }

        // Acumulación: si hay pena pendiente solo se puede apilar la misma penalización
        if (acumulacionActiva && penaAcumulada > 0)
        {
            return ladoCartaJugar->getTipo() == tipoPenaActual;
        }

        Color colorCarta = ladoCartaJugar->getColor();
        Color colorDescarte = ladoDescarte->getColor();
        int numeroCarta = ladoCartaJugar->getNumero();
        int numDescarte = ladoDescarte->getNumero();

        // Comodín siempre puede jugarse
        bool esComodin = (colorCarta == Color::NEGRO);

        // Mismo color activo (puede diferir del descarte si se jugó comodín)
        bool mismoColor = (colorCarta == colorActivo);

        // Mismo número (ambos deben ser >= 0 para comparar)
        bool mismoNumero = (numeroCarta >= 0 && numDescarte >= 0 && numeroCarta == numDescarte);

        // Mismo tipo de carta especial (mismo número == -1 significa mismo tipo especial)
        bool mismoTipo = (numeroCarta == -1 && numDescarte == -1 &&
                          ladoCartaJugar->getTipo() == ladoDescarte->getTipo());

        bool puedeJugar = esComodin || mismoColor || mismoNumero || mismoTipo;

        return puedeJugar;
    }
    catch (...)
    {
        return false;
    }
}
