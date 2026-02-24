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

Juego::Juego(QObject* parent): QObject(parent), ladoOscuroActivo(false),
    indiceTurnoActual(0), sentidoJuego(1){
}


Juego::~Juego(){

    while(!jugadores.estaVacia()){
        Jugador* j = jugadores.obtenerElementoEnPosicion(0);
        delete j;
        jugadores.eliminarDatoEnPosicion(0);
    }
}

void Juego::inicializarMazo(int cantidadJugadores, bool modoFlip){
    qDebug() << "[JUEGO] Iniciando inicializarMazo";

    // Limpiar mazo y descarte anteriores

    while(mazo.getSize() > 0){
        mazo.eliminarDatoEnPosicion(0);
    }
    while(descarte.getSize() > 0){
        descarte.eliminarDatoEnPosicion(0);
    }

    ladoOscuroActivo = false;

    int numMazos = ((cantidadJugadores - 1) / 6) + 1;
    qDebug() << "[JUEGO] Número de mazos a crear:" << numMazos;

    // Crear el mazo según el modo
    if (modoFlip) {
        generarMazoFlipManual(numMazos);
    } else {
        generarMazoNormalManual(numMazos);
    }

    qDebug() << "[JUEGO] Mazo creado con" << mazo.getSize() << "cartas";

    // MEZCLAR EL MAZO COMPLETO
    barajarMazo();
    qDebug() << "[JUEGO] Mazo mezclado";

    // Repartir cartas a jugadores
    repartirCartas();
    qDebug() << "[JUEGO] Cartas repartidas";

    // Sacar primera carta para descarte
    sacarPrimeraCarta();
    qDebug() << "[JUEGO] Primera carta colocada en descarte";

    emit partidaIniciadaSignal();
}

void Juego::sacarPrimeraCarta(){
    if (!mazo.estaVacia()) {
        Carta primeraCarta = robarDelMazo();
        descarte.insertarInicio(primeraCarta);

        // Emitir señal para actualizar UI inmediatamente
        emit descarteActualizadoSignal(QString::fromStdString(
            primeraCarta.getLadoActivo()->getRutaArchivo()));
    }
}

void Juego::mezclarArregloLados(LadoCarta* arreglo[], int tamano) {
    // Algoritmo de Fisher-Yates para barajar el arreglo
    for (int i = tamano - 1; i > 0; i--) {
        // Generar un índice aleatorio entre 0 e i
        int j = rand() % (i + 1);

        // Intercambiar arreglo[i] con arreglo[j]
        LadoCarta* temporal = arreglo[i];
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

void Juego::generarMazoNormalManual(int numMazos){
    qDebug() << "[MAZO NORMAL] Iniciando creación con" << numMazos << "mazos";

    for(int n = 0; n < numMazos; n++){
        Color colores[] = {Color::ROJO, Color::AZUL, Color::VERDE, Color::AMARILLO};

        // === CARTAS NUMÉRICAS ===
        // 1 carta de 0 por color (4 cartas)
        for(Color color : colores){
            std::string rutaNumero0 = ruta.generarRuta(TipoCarta::NUMERO, color, 0);
            mazo.insertarFinal(Carta(new LadoNumero(color, 0, rutaNumero0)));
        }

        // 2 cartas de cada número 1-9 por color (72 cartas)
        for(Color color : colores) {
            for(int numero = 1; numero <= 9; numero++) {
                std::string rutaNumero = ruta.generarRuta(TipoCarta::NUMERO, color, numero);
                // Primera carta
                mazo.insertarFinal(Carta(new LadoNumero(color, numero, rutaNumero)));
                // Segunda carta
                mazo.insertarFinal(Carta(new LadoNumero(color, numero, rutaNumero)));
            }
        }
        qDebug() << "[MAZO NORMAL] Creadas 76 cartas numéricas. Total:" << mazo.getSize();

        // === CARTAS ESPECIALES DE COLOR ===
        // 2 cartas de cada tipo por color
        for (Color color : colores) {
            // 2 cartas +2
            for(int i = 0; i < 2; i++) {
                std::string rutaRoba2 = ruta.generarRuta(TipoCarta::ROBA2, color);
                mazo.insertarFinal(Carta(new LadoRobaDos(color, -1, rutaRoba2)));
            }

            // 2 cartas Bloqueo
            for(int i = 0; i < 2; i++) {
                std::string rutaBloqueo = ruta.generarRuta(TipoCarta::BLOQUEO, color);
                mazo.insertarFinal(Carta(new LadoSalto(color, -1, rutaBloqueo)));
            }

            // 2 cartas Reversa
            for(int i = 0; i < 2; i++) {
                std::string rutaReverse = ruta.generarRuta(TipoCarta::REVERSE, color);
                mazo.insertarFinal(Carta(new LadoReverse(color, -1, rutaReverse)));
            }
        }
        qDebug() << "[MAZO NORMAL] Creadas 24 cartas especiales de color. Total:" << mazo.getSize();

        // === CARTAS COMODÍN ===
        // 4 cartas de cada tipo comodín
        std::string rutaComodinColor = ruta.generarRuta(TipoCarta::COMODIN, Color::NEGRO);
        std::string rutaComodinRoba4 = ruta.generarRuta(TipoCarta::COMODIN4, Color::NEGRO);
        std::string rutaComodinAdivinar = ruta.generarRuta(TipoCarta::ADIVINARCARTA, Color::NEGRO);
        std::string rutaComodinCambiarMano = ruta.generarRuta(TipoCarta::CAMBIARMANO, Color::NEGRO);

        for(int i = 0; i < 4; i++) {
            mazo.insertarFinal(Carta(new LadoComodinColor(Color::NEGRO, -1, rutaComodinColor)));
            mazo.insertarFinal(Carta(new LadoComodinRobarCuatro(Color::NEGRO, -1, rutaComodinRoba4)));
            mazo.insertarFinal(Carta(new LadoAdivinarCarta(Color::NEGRO, -1, rutaComodinAdivinar)));
            mazo.insertarFinal(Carta(new LadoCambiarMano(Color::NEGRO, -1, rutaComodinCambiarMano)));
        }
        qDebug() << "[MAZO NORMAL] Creadas 16 cartas comodín. Total:" << mazo.getSize();
    }

    qDebug() << "[MAZO NORMAL] Mazo completo creado. Total cartas:" << mazo.getSize();
    qDebug() << "[MAZO NORMAL] Esperadas:" << (116 * numMazos);
}



void Juego::generarMazoFlipManual(int numMazos){
    qDebug() << "[MAZO FLIP] Iniciando creación con" << numMazos << "mazos";

    for(int n = 0; n < numMazos; n++){
        qDebug() << "[MAZO FLIP] Creando mazo" << (n+1) << "de" << numMazos;

        Color cClaros[] = {Color::ROJO, Color::AZUL, Color::VERDE, Color::AMARILLO};
        Color cOscuros[] = {Color::ROSA, Color::TURQUESA, Color::NARANJA, Color::PURPURA};

        // === CARTAS NUMÉRICAS === (76 cartas: 38 cartas x 2 lados = 76 efectos)

        // 1 carta de 0 por cada color (4 cartas)
        for(int i = 0; i < 4; i++){
            LadoCarta* ladoClaro = new LadoNumero(cClaros[i], 0, ruta.generarRuta(TipoCarta::NUMERO, cClaros[i], 0));
            LadoCarta* ladoOscuro = new LadoNumero(cOscuros[i], 0, ruta.generarRuta(TipoCarta::NUMERO, cOscuros[i], 0));
            mazo.insertarFinal(Carta(ladoClaro, ladoOscuro));
        }
        qDebug() << "[MAZO FLIP] Creadas 4 cartas de 0. Total mazo:" << mazo.getSize();

        // 2 cartas de cada número 1-9 por color (36 cartas)
        for(int color = 0; color < 4; color++){
            for(int numero = 1; numero <= 9; numero++){
                // Primera carta del número
                LadoCarta* ladoClaro1 = new LadoNumero(cClaros[color], numero, ruta.generarRuta(TipoCarta::NUMERO, cClaros[color], numero));
                LadoCarta* ladoOscuro1 = new LadoNumero(cOscuros[color], numero, ruta.generarRuta(TipoCarta::NUMERO, cOscuros[color], numero));
                mazo.insertarFinal(Carta(ladoClaro1, ladoOscuro1));

                // Segunda carta del número
                LadoCarta* ladoClaro2 = new LadoNumero(cClaros[color], numero, ruta.generarRuta(TipoCarta::NUMERO, cClaros[color], numero));
                LadoCarta* ladoOscuro2 = new LadoNumero(cOscuros[color], numero, ruta.generarRuta(TipoCarta::NUMERO, cOscuros[color], numero));
                mazo.insertarFinal(Carta(ladoClaro2, ladoOscuro2));
            }
        }
        qDebug() << "[MAZO FLIP] Creadas 72 cartas numéricas 1-9. Total mazo:" << mazo.getSize();
        // Total numéricas: 76 cartas

        // === CARTAS ESPECIALES DE COLOR === (32 cartas)

        for(int color = 0; color < 4; color++){
            // 2 cartas: +1 (claro) / +3 (oscuro)
            for(int i = 0; i < 2; i++){
                LadoCarta* ladoRoba1 = new LadoRobaUno(cClaros[color], -1, ruta.generarRuta(TipoCarta::ROBA1, cClaros[color]));
                LadoCarta* ladoRoba3 = new LadoRobaTres(cOscuros[color], -1, ruta.generarRuta(TipoCarta::ROBA3, cOscuros[color]));
                mazo.insertarFinal(Carta(ladoRoba1, ladoRoba3));
            }

            // 2 cartas: Cambio dirección (ambos lados)
            for(int i = 0; i < 2; i++){
                LadoCarta* ladoCambioClaro = new LadoCambiarDireccion(cClaros[color], -1, ruta.generarRuta(TipoCarta::CAMBIARDIRECCION, cClaros[color]));
                LadoCarta* ladoCambioOscuro = new LadoCambiarDireccion(cOscuros[color], -1, ruta.generarRuta(TipoCarta::CAMBIARDIRECCION, cOscuros[color]));
                mazo.insertarFinal(Carta(ladoCambioClaro, ladoCambioOscuro));
            }

            // 2 cartas: Bloqueo (claro) / Salto todos (oscuro)
            for(int i = 0; i < 2; i++){
                LadoCarta* ladoBloqueo = new LadoSalto(cClaros[color], -1, ruta.generarRuta(TipoCarta::BLOQUEO, cClaros[color]));
                LadoCarta* ladoSaltoTodos = new LadoSaltoTodos(cOscuros[color], -1, ruta.generarRuta(TipoCarta::SALTODOS, cOscuros[color]));
                mazo.insertarFinal(Carta(ladoBloqueo, ladoSaltoTodos));
            }

            // 2 cartas: FLIP (ambos lados)
            for(int i = 0; i < 2; i++){
                LadoCarta* ladoFlipClaro = new LadoFlip(cClaros[color], -1, ruta.generarRuta(TipoCarta::FLIP, cClaros[color]));
                LadoCarta* ladoFlipOscuro = new LadoFlip(cOscuros[color], -1, ruta.generarRuta(TipoCarta::FLIP, cOscuros[color]));
                mazo.insertarFinal(Carta(ladoFlipClaro, ladoFlipOscuro));
            }
        }
        qDebug() << "[MAZO FLIP] Creadas 32 cartas especiales de color. Total mazo:" << mazo.getSize();

        // === CARTAS COMODÍN === (24 cartas)

        // 4 cartas: +2 (claro) / +6 (oscuro)
        for(int i = 0; i < 4; i++){
            LadoCarta* ladoComodin2 = new LadoComodinRobarDos(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::COMODIN2, Color::NEGRO));
            LadoCarta* ladoComodin6 = new LadoComodinRobarSeis(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::COMODIN6, Color::NEGRO));
            mazo.insertarFinal(Carta(ladoComodin2, ladoComodin6));
        }

        // 4 cartas: Cambio color (claro) / Color eterno (oscuro)
        for(int i = 0; i < 4; i++){
            LadoCarta* ladoComodinColor = new LadoComodinColor(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::COMODIN, Color::NEGRO));
            LadoCarta* ladoColorEterno = new LadoColorEterno(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::COLORETERNO, Color::NEGRO));
            mazo.insertarFinal(Carta(ladoComodinColor, ladoColorEterno));
        }

        // 4 cartas: Adivinar carta (ambos lados)
        for(int i = 0; i < 4; i++){
            LadoCarta* ladoAdivinarClaro = new LadoAdivinarCarta(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::ADIVINARCARTA, Color::NEGRO));
            LadoCarta* ladoAdivinarOscuro = new LadoAdivinarCarta(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::ADIVINARCARTA, Color::NEGRO));
            mazo.insertarFinal(Carta(ladoAdivinarClaro, ladoAdivinarOscuro));
        }

        // 4 cartas: Cambiar mano (ambos lados)
        for(int i = 0; i < 4; i++){
            LadoCarta* ladoCambiarClaro = new LadoCambiarMano(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::CAMBIARMANO, Color::NEGRO));
            LadoCarta* ladoCambiarOscuro = new LadoCambiarMano(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::CAMBIARMANO, Color::NEGRO));
            mazo.insertarFinal(Carta(ladoCambiarClaro, ladoCambiarOscuro));
        }

        // 4 cartas: Eclipse (claro) / Espía (oscuro)
        for(int i = 0; i < 4; i++){
            LadoCarta* ladoAdivinarCarta = new LadoAdivinarCarta(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::ADIVINARCARTA, Color::NEGRO));
            LadoCarta* ladoCambiarMano = new LadoCambiarMano(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::CAMBIARMANO, Color::NEGRO));
            mazo.insertarFinal(Carta(ladoAdivinarCarta, ladoCambiarMano));
        }

        // 4 cartas: +4 (claro) - estas parecen ser especiales adicionales
        for(int i = 0; i < 4; i++){
            LadoCarta* ladoComodin4Claro = new LadoComodinRobarCuatro(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::COMODIN4, Color::NEGRO));
            LadoCarta* ladoComodin4Oscuro = new LadoComodinRobarCuatro(Color::NEGRO, -1, ruta.generarRuta(TipoCarta::COMODIN4, Color::NEGRO));
            mazo.insertarFinal(Carta(ladoComodin4Claro, ladoComodin4Oscuro));
        }

        qDebug() << "[MAZO FLIP] Creadas 24 cartas comodín. Total mazo:" << mazo.getSize();
    }

    qDebug() << "[MAZO FLIP] Mazo completo creado. Total cartas:" << mazo.getSize();
    qDebug() << "[MAZO FLIP] Esperadas:" << (132 * numMazos);
}
   void Juego::avanzarTurno() {

       // Avanzamos el turno según el sentido (1 o -1)
       indiceTurnoActual += sentidoJuego;

       int numJugadores = jugadores.getSize();

       // si salimos de los límites aplicado a una lista circular
       if (indiceTurnoActual >= numJugadores) {
           indiceTurnoActual = 0;
       } else if (indiceTurnoActual < 0) {
           indiceTurnoActual = numJugadores - 1;
       }
   }


   Carta Juego::robarDelMazo(){
       if(mazo.estaVacia()){
           return nullptr;
       }
       return mazo.robarCarta();
   }

   bool Juego::getLadoOscuroActivo()const{
       return ladoOscuroActivo;
   }

   void Juego::setLadoOscuroActivo(bool estado){
       ladoOscuroActivo = estado;
   }

   int Juego::getTamanoMazo()const{
       return mazo.getSize();
   }


ListaGenerica<Jugador*>& Juego:: getJugadores(){
    return jugadores;
}

ListaGenerica<Carta>& Juego::getMazo(){
    return mazo;
}

ListaGenerica<Carta>& Juego::getDescarte(){
    return descarte;
}

int Juego::getTamanoDescarte() const{
    return descarte.getSize();
}

Jugador* Juego::getJugadorEnPosicion(int indice){
    return jugadores.obtenerElementoEnPosicion(indice);
}

int Juego::getCantidadJugadores() const{
    return jugadores.getSize();
}

int Juego::getSentidoJuego() const{
    return sentidoJuego;
}

void Juego::setSentidoJuego(int sentido){
    sentidoJuego = sentido;
}

void Juego::agregarADescarte(Carta carta){
    descarte.insertarInicio(carta);
}


//* Agregar la UI para refrescarla cada vez que se realiza una accion
void Juego::jugarCarta(Jugador* jugador, int indiceCartaEnMano,
                       const std::string& jugadorSeleccionado,
                       int numeroAdivinado,
                       const std::string& colorAdivinado) {

    qDebug() << "[JUEGO] === jugarCarta INICIADO ===";
    qDebug() << "[JUEGO] Índice carta:" << indiceCartaEnMano;

    // Verificaciones de seguridad
    if (!jugador) {
        qDebug() << "[JUEGO ERROR] Jugador es nullptr";
        return;
    }

    if (indiceCartaEnMano < 0 || indiceCartaEnMano >= jugador->getMano().getSize()) {
        qDebug() << "[JUEGO ERROR] Índice de carta fuera de rango";
        return;
    }

    try {
        // Obtener la carta de forma segura
        Carta cartaJugada = jugador->getMano().obtenerElementoEnPosicion(indiceCartaEnMano);
        if (!cartaJugada.esValida()) {
            qDebug() << "[JUEGO ERROR] Carta no válida";
            return;
        }

        qDebug() << "[JUEGO] Removiendo carta de la mano del jugador";

        // El jugador tira la carta (la quitamos de su mano)
        jugador->getMano().eliminarDatoEnPosicion(indiceCartaEnMano);

        qDebug() << "[JUEGO] Agregando carta al descarte";

        // La ponemos en la pila de descarte
        descarte.insertarInicio(cartaJugada);

        qDebug() << "[JUEGO] Emitiendo señales de actualización";

        // EMITIR SEÑALES: La carta fue jugada
        emit cartaJugadaSignal(indiceCartaEnMano);
        emit manoActualizadaSignal();

        // Actualizar información del descarte
        LadoCarta* ladoActivo = cartaJugada.getLadoActivo();
        if (ladoActivo) {
            colorActivo = ladoActivo->getColor();
            QString ruta = QString::fromStdString(ladoActivo->getRutaArchivo());
            emit descarteActualizadoSignal(ruta);
        }

        qDebug() << "[JUEGO] Aplicando efecto de la carta";

        // Procesamos qué hace esa carta
        aplicarEfectoCarta(cartaJugada, jugadorSeleccionado, numeroAdivinado, colorAdivinado);

        qDebug() << "[JUEGO] Efecto aplicado, avanzando turno";

        // Preparamos el turno del siguiente jugador
        avanzarTurno();

        // EMITIR SEÑAL DE TURNO CAMBIADO
        Jugador* nuevoJugador = getJugadorActual();
        if (nuevoJugador) {
            emit turnoCambiadoSignal(QString::fromStdString(nuevoJugador->getNombreJugador()));
            emit manoActualizadaSignal(); // La mano del nuevo jugador
        }

        // Actualizar estado del mazo
        emit mazoActualizadoSignal(mazo.getSize());

        qDebug() << "[JUEGO] === jugarCarta COMPLETADO ===";

    } catch (const std::exception& e) {
        qDebug() << "[JUEGO ERROR CRÍTICO] Excepción estándar en jugarCarta:" << e.what();
    } catch (...) {
        qDebug() << "[JUEGO ERROR CRÍTICO] Excepción desconocida en jugarCarta";
    }
}

   void Juego::repartirCartas(int cartasPorJugador) {

       // Verificar que hay jugadores
       if (jugadores.estaVacia()) {
           return;
       }

       // Verificar que hay suficientes cartas en el mazo
       int cartasNecesarias = jugadores.getSize() * cartasPorJugador;
       if (mazo.getSize() < cartasNecesarias) {
           return;
       }

       // Repartir cartas a cada jugador
       for (int i = 0; i < jugadores.getSize(); i++) {
           Jugador* jugadorActual = jugadores.obtenerElementoEnPosicion(i);

           for (int j = 0; j < cartasPorJugador; j++) {
               Carta cartaARobar = robarDelMazo();
               if (cartaARobar.esValida()) jugadorActual->agregarCarta(cartaARobar);

           }
       }

       // Colocar la primera carta en el descarte para comenzar el juego
       if (!mazo.estaVacia()) {
           Carta primeraCarta = robarDelMazo();
           descarte.insertarInicio(primeraCarta);

           // Actualizar el color activo según la primera carta
           if (primeraCarta.esValida()) {
               colorActivo = primeraCarta.getLadoActivo()->getColor();

               // Si es comodín se elige un color
               if (colorActivo == Color::NEGRO) {
                   // Elegir un color aleatorio
                   Color coloresNormales[] = {Color::ROJO, Color::AZUL,
                                              Color::VERDE, Color::AMARILLO};
                   colorActivo = coloresNormales[rand() % 4];
               }
           }
       }
   }

   Jugador* Juego::getJugadorActual(){
       return jugadores.obtenerElementoEnPosicion(indiceTurnoActual);
   }

   Jugador* Juego::getJugadorSeleccionado(const std::string& nombreJugador){
       for(int i = 0; i < jugadores.getSize(); i++){
           Jugador* temp = jugadores.obtenerElementoEnPosicion(i);
           if(temp->getNombreJugador() == nombreJugador) return temp;
       }
       return nullptr;
   }

   Color Juego::convertirStringAColor(const std::string& colorStr) {
       if (colorStr == "Rojo") return Color::ROJO;
       if (colorStr == "Azul") return Color::AZUL;
       if (colorStr == "Verde") return Color::VERDE;
       if (colorStr == "Amarillo") return Color::AMARILLO;
       // Si estás en modo oscuro
       if (colorStr == "Rosa") return Color::ROSA;
       if (colorStr == "Turquesa") return Color::TURQUESA;
       if (colorStr == "Naranja") return Color::NARANJA;
       if (colorStr == "Purpura") return Color::PURPURA;
       // Valor por defecto
       return Color::NEGRO;
   }

   void Juego::barajarMazo()
   {
       mazo.barajar();
   }



   void Juego::barajarDescarte(){
       while(descarte.getSize()>1){
           Carta carta = descarte.robarCarta();
           mazo.insertarFinal(carta);
       }
       barajarMazo();
   }

   Color Juego::getColorActivo() const {
       return colorActivo;
   }

   void Juego::setColorActivo(Color nuevoColor) {
       this->colorActivo = nuevoColor;
   }

   bool Juego::mazoEstaVacio()const{
       return mazo.getSize()<=0;
   }

   void Juego::setJugadorEnLista(const std::string nombreJugador){
       Jugador* jugador = new Jugador(nombreJugador);
       jugadores.insertarFinal(jugador);
   }

   void Juego::onCartaJugadaSlot(int indiceCarta) {
       qDebug() << "[JUEGO] === onCartaJugadaSlot INICIADO ===";
       qDebug() << "[JUEGO] Índice carta recibido:" << indiceCarta;

       // Bloquear múltiples llamadas simultáneas
       static bool procesando = false;
       if (procesando) {
           qDebug() << "[JUEGO] Ya procesando otra carta, ignorando";
           return;
       }

       procesando = true;

       try {
           // Validaciones básicas
           Jugador* jugadorActual = getJugadorActual();
           if (!jugadorActual || indiceCarta < 0 || indiceCarta >= jugadorActual->getMano().getSize()) {
               qDebug() << "[JUEGO ERROR] Validaciones fallidas";
               procesando = false;
               return;
           }

           // Jugar la carta SIN emitir señales inmediatas
           qDebug() << "[JUEGO] Llamando a jugarCartaSinSeñales";
           jugarCartaSinSeñales(jugadorActual, indiceCarta, "", -1, "");

           // Emitir señales DESPUÉS de un delay para evitar ciclos
           QTimer::singleShot(50, this, [this]() {
               emit manoActualizadaSignal();

               Jugador* nuevoJugador = getJugadorActual();
               if (nuevoJugador) {
                   emit turnoCambiadoSignal(QString::fromStdString(nuevoJugador->getNombreJugador()));
               }

               emit mazoActualizadoSignal(mazo.getSize());
           });

       } catch (...) {
           qDebug() << "[JUEGO ERROR CRÍTICO] Excepción en onCartaJugadaSlot";
       }

       procesando = false;
       qDebug() << "[JUEGO] === onCartaJugadaSlot FINALIZADO ===";
   }


   bool Juego::puedeJugarCarta(const Carta& carta) {
       qDebug() << "[JUEGO] Verificando si se puede jugar carta";

       if (!carta.esValida()) {
           qDebug() << "[JUEGO] Carta no válida";
           return false;
       }

       if (descarte.estaVacia()) {
           qDebug() << "[JUEGO] No hay cartas en descarte";
           return true; // Primera carta siempre se puede jugar
       }

       try {
           Carta cartaEnDescarte = descarte.obtenerElementoEnPosicion(0);
           if (!cartaEnDescarte.esValida()) {
               qDebug() << "[JUEGO] Carta en descarte no válida";
               return true; // Si no hay carta válida en descarte, se puede jugar cualquiera
           }

           LadoCarta* ladoCartaJugar = carta.getLadoActivo();
           LadoCarta* ladoDescarte = cartaEnDescarte.getLadoActivo();

           if (!ladoCartaJugar || !ladoDescarte) {
               qDebug() << "[JUEGO ERROR] Algún lado de carta es nullptr";
               return false;
           }

           // Verificar compatibilidad
           bool mismoColor = (ladoCartaJugar->getColor() == ladoDescarte->getColor());
           bool mismoNumero = (ladoCartaJugar->getNumero() == ladoDescarte->getNumero());
           bool esComodin = (ladoCartaJugar->getColor() == Color::NEGRO);

           bool puedeJugar = mismoColor || mismoNumero || esComodin;

           qDebug() << "[JUEGO] Puede jugar carta:" << puedeJugar;
           qDebug() << "[JUEGO] Mismo color:" << mismoColor;
           qDebug() << "[JUEGO] Mismo número:" << mismoNumero;
           qDebug() << "[JUEGO] Es comodín:" << esComodin;

           return puedeJugar;

       } catch (...) {
           qDebug() << "[JUEGO ERROR] Excepción al verificar si se puede jugar carta";
           return false;
       }
   }


   void Juego::aplicarEfectoCarta(Carta cartaJugada, const std::string jugadorSeleccionado,
                                  int numeroSeleccionado, const std::string& colorSeleccionado) {

       qDebug() << "[JUEGO] === Iniciando aplicarEfectoCarta ===";

       if (!cartaJugada.esValida()) {
           qDebug() << "[JUEGO ERROR] Carta inválida recibida";
           return;
       }

       LadoCarta* ladoActual = cartaJugada.getLadoActivo();
       if (!ladoActual) {
           qDebug() << "[JUEGO ERROR] Lado actual es nullptr";
           return;
       }

       qDebug() << "[JUEGO] Aplicando efecto de carta tipo:" << static_cast<int>(ladoActual->getTipo());

       try {
           // Llamar al método aplicarEfecto del lado activo de la carta
           ladoActual->aplicarEfecto(this, colorSeleccionado, jugadorSeleccionado, numeroSeleccionado);
           qDebug() << "[JUEGO] Efecto aplicado correctamente";
       } catch (...) {
           qDebug() << "[JUEGO ERROR] Excepción al aplicar efecto de carta";
       }

       qDebug() << "[JUEGO] === Finalizó aplicarEfectoCarta ===";
   }



   void Juego::jugarCartaSinSeñales(Jugador* jugador, int indiceCartaEnMano,
                                    const std::string& jugadorSeleccionado,
                                    int numeroAdivinado,
                                    const std::string& colorAdivinado) {

       qDebug() << "[JUEGO] === jugarCartaSinSeñales INICIADO ===";

       if (!jugador || indiceCartaEnMano < 0 || indiceCartaEnMano >= jugador->getMano().getSize()) {
           qDebug() << "[JUEGO ERROR] Parámetros inválidos";
           return;
       }

       try {
           // Obtener y remover carta
           Carta cartaJugada = jugador->getMano().obtenerElementoEnPosicion(indiceCartaEnMano);
           jugador->getMano().eliminarDatoEnPosicion(indiceCartaEnMano);

           // Agregar al descarte
           descarte.insertarInicio(cartaJugada);

           // Actualizar color activo
           LadoCarta* ladoActivo = cartaJugada.getLadoActivo();
           if (ladoActivo) {
               colorActivo = ladoActivo->getColor();
           }

           // Aplicar efecto (SIN emitir señales desde aquí)
           aplicarEfectoCarta(cartaJugada, jugadorSeleccionado, numeroAdivinado, colorAdivinado);

           // Avanzar turno
           avanzarTurno();

           qDebug() << "[JUEGO] === jugarCartaSinSeñales COMPLETADO ===";

       } catch (...) {
           qDebug() << "[JUEGO ERROR CRÍTICO] Excepción en jugarCartaSinSeñales";
       }
   }
