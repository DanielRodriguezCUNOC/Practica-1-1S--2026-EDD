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

Juego::Juego(): ladoOscuroActivo(false), indiceTurnoActual(0), sentidoJuego(1){
}


Juego::~Juego(){

    while(!jugadores.estaVacia()){
        Jugador* j = jugadores.obtenerElementoEnPosicion(0);
        delete j;
        jugadores.eliminarDatoEnPosicion(0);
    }
}

void Juego::inicializarMazo(int cantidadJugadores, bool modoFlip){
    ladoOscuroActivo = false;

    int numMazos = ((cantidadJugadores -1)/6)+1;

    modoFlip ? generarMazoFlipManual(numMazos) : generarMazoNormalManual(numMazos);

    barajarMazo();

    repartirCartas();

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
    for(int n = 0; n <numMazos; n++){

    Color colores[] = {Color::ROJO, Color::AZUL, Color::VERDE, Color::AMARILLO};
        Color c;
        // 1 Cero por color
        for(Color color : colores){
        std::string rutaNumero0 = ruta.generarRuta(TipoCarta::NUMERO, color, 0);
        mazo.insertarFinal(Carta(new LadoNumero(color, 0, rutaNumero0)));
        }

        // 2 veces del 1-9
        for (int i = 1; i <= 36; i++) {

            c= colores[i/9]; // j=1 -> 1/9 = 0 -> Rojo
            int numero = (i%9) + 1; // si j = 4 (4%9) = 4 -> 4+1-> 5
            std::string rutaNumero = ruta.generarRuta(TipoCarta::NUMERO, c, numero);

            mazo.insertarFinal( Carta(new LadoNumero(c, numero, rutaNumero)));
            mazo.insertarFinal( Carta(new LadoNumero(c, numero, rutaNumero)));
        }

         // 2 veces Acciones (Roba2, Salto, Reverse)
        for (Color c : colores) {

            std::string rutaAccion;

            rutaAccion = ruta.generarRuta(TipoCarta::ROBA2, c);
            mazo.insertarFinal( Carta(new LadoRobaDos(c, -1, rutaAccion)));
            mazo.insertarFinal( Carta(new LadoRobaDos(c, -1, rutaAccion)));

            rutaAccion = ruta.generarRuta(TipoCarta::BLOQUEO, c);
            mazo.insertarFinal( Carta(new LadoSalto(c, -1, rutaAccion)));
            mazo.insertarFinal( Carta(new LadoSalto(c, -1, rutaAccion)));

            rutaAccion = ruta.generarRuta(TipoCarta::REVERSE, c);
            mazo.insertarFinal( Carta(new LadoReverse(c, -1, rutaAccion)));
            mazo.insertarFinal( Carta(new LadoReverse(c, -1, rutaAccion)));
        }

    std::string rutaComodinColor =
        ruta.generarRuta(TipoCarta::COMODIN, Color::NEGRO);
    std::string rutaComodinRoba4 =
        ruta.generarRuta(TipoCarta::COMODIN4, Color::NEGRO);
    std::string rutaComodinAdivinar =
        ruta.generarRuta(TipoCarta::ADIVINARCARTA, Color::NEGRO);
    std::string rutaComodinCambiarMano =
        ruta.generarRuta(TipoCarta::CAMBIARMANO, Color::NEGRO);

    // 4 Comodines Color
    for(int i=0;i<4;i++)
        mazo.insertarFinal( Carta(new LadoComodinColor(Color::NEGRO, -1, rutaComodinColor)));

    // 4 +4
    for(int i=0;i<4;i++)
        mazo.insertarFinal( Carta(new LadoComodinRobarCuatro(Color::NEGRO, -1, rutaComodinRoba4)));

    // 4 Adivinar
    for(int i=0;i<4;i++)
        mazo.insertarFinal( Carta(new LadoAdivinarCarta(Color::NEGRO, -1, rutaComodinAdivinar)));

    // 4 Cambiar Mano
    for(int i=0;i<4;i++)
        mazo.insertarFinal( Carta(new LadoCambiarMano(Color::NEGRO, -1, rutaComodinCambiarMano)));

    }


}

void Juego::generarMazoFlipManual(int numMazos){
    //* Cartas totales que debe tener un mazo de UNO FLIP
    int const CARTAS_POR_MAZO = 132;
    int cartasTotales = CARTAS_POR_MAZO * numMazos;

    // Arreglos dinámicos temporales para guardar los punteros a cada lado
    LadoCarta** ladosClaros = new LadoCarta*[cartasTotales];
    LadoCarta** ladosOscuros = new LadoCarta*[cartasTotales];

    // Contadores para saber en qué posición del arreglo vamos
    int idxClaro = 0;
    int idxOscuro = 0;

    Color cClaros[] = {Color::ROJO, Color::AZUL, Color::VERDE, Color::AMARILLO};
    Color cOscuros[] = {Color::ROSA, Color::TURQUESA, Color::NARANJA, Color::PURPURA};

    Color claro = Color::NEGRO;
    Color oscuro = Color::NEGRO;

    for(int n = 0; n < numMazos; n++){

        // 4 Numero 0 por lado
        for(Color color : cClaros){
            ladosClaros[idxClaro++] = new LadoNumero(color, 0, ruta.generarRuta(TipoCarta::NUMERO, color, 0));
        }

        // 4 Numero 0 por lado
        for(Color color : cOscuros){
            ladosOscuros[idxOscuro++] = new LadoNumero(color, 0, ruta.generarRuta(TipoCarta::NUMERO, color, 0));
        }

        // 2 veces del 1-9 por lado
        for (int i = 1; i <= 36; i++) {
            claro = cClaros[i/9];
            oscuro = cOscuros[i/9];
            int numero = (i%9) + 1;

            ladosClaros[idxClaro++] = new LadoNumero(claro, numero, ruta.generarRuta(TipoCarta::NUMERO, claro, numero));
            ladosClaros[idxClaro++] = new LadoNumero(claro, numero, ruta.generarRuta(TipoCarta::NUMERO, claro, numero));

            ladosOscuros[idxOscuro++] = new LadoNumero(oscuro, numero, ruta.generarRuta(TipoCarta::NUMERO, oscuro, numero));
            ladosOscuros[idxOscuro++] = new LadoNumero(oscuro, numero, ruta.generarRuta(TipoCarta::NUMERO, oscuro, numero));
        }

        // 2 flip por cada color de cada lado

        for (int i = 0; i < 4; i++) {

            claro = cClaros[i];
            ladosClaros[idxClaro++] = new LadoFlip(claro, -1, ruta.generarRuta(TipoCarta::FLIP, claro));
            ladosClaros[idxClaro++] = new LadoFlip(claro, -1, ruta.generarRuta(TipoCarta::FLIP, claro));

            oscuro = cOscuros[i];
            ladosOscuros[idxOscuro++] = new LadoFlip(oscuro, -1, ruta.generarRuta(TipoCarta::FLIP, oscuro));
            ladosOscuros[idxOscuro++] = new LadoFlip(oscuro, -1, ruta.generarRuta(TipoCarta::FLIP, oscuro));
        }



        // 2 veces Roba1 por color claro
        for(Color color: cClaros){
            ladosClaros[idxClaro++] = new LadoRobaUno(color, -1, ruta.generarRuta(TipoCarta::ROBA1, color));
            ladosClaros[idxClaro++] = new LadoRobaUno(color, -1, ruta.generarRuta(TipoCarta::ROBA1, color));
        }
        // 2 veces Roba3 por color oscuro
        for(Color color: cOscuros){
            ladosOscuros[idxOscuro++] =new LadoRobaTres(color, -1, ruta.generarRuta(TipoCarta::ROBA3, color));
            ladosOscuros[idxOscuro++] = new LadoRobaTres(color, -1, ruta.generarRuta(TipoCarta::ROBA3, color));
        }

        // 2 veces cambio de direccion por lado
        for (int i = 0; i < 4; i++) {

            claro = cClaros[i];
            ladosClaros[idxClaro++] = new LadoCambiarDireccion(claro, -1, ruta.generarRuta(TipoCarta::CAMBIARDIRECCION, claro));
            ladosClaros[idxClaro++]= new LadoCambiarDireccion(claro, -1, ruta.generarRuta(TipoCarta::CAMBIARDIRECCION, claro));

            oscuro = cOscuros[i];
            ladosOscuros[idxOscuro++] = new LadoCambiarDireccion(oscuro, -1, ruta.generarRuta(TipoCarta::CAMBIARDIRECCION, oscuro));
            ladosOscuros[idxOscuro++] = new LadoCambiarDireccion(oscuro, -1, ruta.generarRuta(TipoCarta::CAMBIARDIRECCION, oscuro));
        }

        // 2 veces bloqueo lado claro | 2 veces salto todo lado oscuro
        for (int i = 0; i < 4; i++) {

            claro = cClaros[i];
             ladosClaros[idxClaro++] = new LadoSalto(claro, -1, ruta.generarRuta(TipoCarta::BLOQUEO, claro));
             ladosClaros[idxClaro++] = new LadoSalto(claro, -1, ruta.generarRuta(TipoCarta::BLOQUEO, claro));

            oscuro = cOscuros[i];
            ladosOscuros[idxOscuro++] = new LadoSaltoTodos(oscuro, -1, ruta.generarRuta(TipoCarta::SALTODOS, oscuro));
            ladosOscuros[idxOscuro++] = new LadoSaltoTodos(oscuro, -1, ruta.generarRuta(TipoCarta::SALTODOS, oscuro));
        }

        // 4 +2 comodin lado claro | 4 +6 comodin lado oscuro
        for (int i = 0; i < 4; i++) {

            ladosClaros[idxClaro++] = new LadoComodinRobarDos(claro, -1, ruta.generarRuta(TipoCarta::COMODIN2, claro));

            ladosOscuros[idxOscuro++] = new LadoComodinRobarSeis(oscuro, -1, ruta.generarRuta(TipoCarta::COMODIN6, oscuro));
        }

        // 4 cambio color comodin lado claro | 4 color eterno comodin lado oscuro
        for (int i = 0; i < 4; i++) {

            ladosClaros[idxClaro++] = new LadoComodinColor(claro, -1, ruta.generarRuta(TipoCarta::COMODIN, claro));
            ladosOscuros[idxOscuro++] = new LadoColorEterno(oscuro, -1, ruta.generarRuta(TipoCarta::COLORETERNO, oscuro));
        }

        // 4 cartas personalizadas por lado por color -> 4*2*2->16
        for (int i = 0; i < 4; i++) {

            ladosClaros[idxClaro++] = new LadoAdivinarCarta(claro, -1, ruta.generarRuta(TipoCarta::ADIVINARCARTA, claro));
            ladosOscuros[idxOscuro++] = new LadoAdivinarCarta(oscuro, -1, ruta.generarRuta(TipoCarta::ADIVINARCARTA, oscuro));


            ladosClaros[idxClaro++] = new LadoCambiarMano(claro, -1, ruta.generarRuta(TipoCarta::CAMBIARMANO, claro));
            ladosOscuros[idxOscuro++] = new LadoCambiarMano(oscuro, -1, ruta.generarRuta(TipoCarta::CAMBIARMANO, oscuro));
        }

    }
    // Mezclar aleatoriamente SOLO los lados oscuros
    mezclarArregloLados(ladosOscuros, idxOscuro);

    // Unir ambos lados en un objeto Carta y agregarlo al mazo final
    for(int i = 0; i < idxClaro; i++){
        mazo.insertarFinal(Carta(ladosClaros[i], ladosOscuros[i]));
    }

    delete[] ladosClaros;
    delete[] ladosOscuros;
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

void Juego::aplicarEfectoCarta(Carta cartaJugada, const std::string jugadorSeleccionado, int numeroSeleccionado, const std::string& colorSeleccionado) {
    LadoCarta* ladoActual = cartaJugada.getLadoActivo();

    if(ladoActual!=nullptr) ladoActual->aplicarEfecto(this,colorSeleccionado);

}

//* Agregar la UI para refrescarla cada vez que se realiza una accion
   void Juego::jugarCarta(Jugador* jugador, int indiceCartaEnMano,
                          const std::string& jugadorSeleccionado,
                          int numeroAdivinado,
                          const std::string& colorAdivinado) {

       // El jugador tira la carta (la quitamos de su mano)
       Carta cartaJugada = jugador->getMano().obtenerElementoEnPosicion(indiceCartaEnMano);
       jugador->getMano().eliminarDatoEnPosicion(indiceCartaEnMano);

       // La ponemos en la pila de descarte
       descarte.insertarInicio(cartaJugada);

       // Procesamos qué hace esa carta
       aplicarEfectoCarta(cartaJugada, jugadorSeleccionado, numeroAdivinado, colorAdivinado);

       // Preparamos el turno del siguiente jugador
       avanzarTurno();
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

