#include "juego.h"
#include "cartanormal.h"
#include "cartaflip.h"
#include <QWidget>
Juego::Juego(): ladoOscuroActivo(false), indiceTurnoActual(0), sentidoJuego(1){
}


void destruirListaCartas(ListaGenerica<Carta*>& lista){
    while(!lista.estaVacia()){
        delete lista.robarCarta();
    }
}

Juego::~Juego(){
    destruirListaCartas(mazo);
    destruirListaCartas(descarte);

    while(!jugadores.estaVacia()){
        Jugador* j = jugadores.obtenerElementoEnPosicion(0);
        delete j;
        jugadores.eliminarDatoEnPosicion(0);
    }
}

/*
 * Juego::~Juego(){
    //* borrar mazos
    while(!mazo.estaVacia()){
        delete mazo.robarCarta();
    }

    //* borrar mano de jugadores
    while(!jugadores.estaVacia()){
        Jugador* j = jugadores.obtenerElementoEnPosicion(0);

        // borrar cartas de la mano del jugador
        auto& mano = j->getMano();
        while(mano.getSize() > 0){
            delete mano.obtenerElementoEnPosicion(0);
            mano.eliminarDatoEnPosicion(0);
        }

        delete j;
        jugadores.eliminarDatoEnPosicion(0);
    }

    //* borrar la pila de descarte
    while(!descarte.estaVacia()){
        delete descarte.robarCarta();
    }
}*/

Carta* Juego::robarDelMazo(){
    if(mazo.estaVacia()){
        return nullptr;
    }
    return mazo.robarCarta();
}

bool Juego::esLadoOscuro()const{
    return ladoOscuroActivo;
}

void Juego::setLadoOscuro(bool estado){
    ladoOscuroActivo = estado;
}

int Juego::getCantidadCartasMazo(){
    return mazo.getSize();
}

void Juego::inicializarMazo(bool modoFlip, int cantidadJugadores){


    while(!mazo.estaVacia()){
        Carta* c = mazo.robarCarta();
        delete c;
    }
    ladoOscuroActivo = false;

    int numMazos = ((cantidadJugadores -1)/6)+1;

    modoFlip ? generarMazoFlipManual(numMazos) : generarMazoNormalManual(numMazos);

    barajarMazo();
}

void Juego::mezclarArregloLados(LadoCarta* arreglo, int size){
    for (int i = size-1; i >0; i--) {
        int j = rand() % (i+1);
        LadoCarta temporal = arreglo[i];
        arreglo[i] = arreglo[j];
        arreglo[j] = temporal;
    }
}

void Juego::mezclarArregloCartas(Carta** arreglo, int size){
    for(int i = size -1; i>0; i--){
        int j = rand() % (i+1);
        Carta* temporal = arreglo[i];
        arreglo[i] = arreglo[j];
        arreglo[j] = temporal;
    }
}

void Juego::generarMazoNormalManual(int numMazos){

    for(int n = 0; n <numMazos; n++){

    Color colores[] = {Color::Rojo, Color::Azul, Color::Verde, Color::Amarillo};

    for (int i = 0; i < 4; i++) {
        Color c = colores[i];

        // 1 Cero por color
        std::string numero0 = ruta.generarRuta(TipoCarta::Numero, c, 0);
        mazo.insertarFinal(new CartaNormal(LadoCarta(TipoCarta::Numero, c, 0, numero0)));

        // 2 veces del 1-9
        for (int n = 1; n <= 9; n++) {
            std::string rutaNumero = ruta.generarRuta(TipoCarta::Numero, c, n);
            mazo.insertarFinal(new CartaNormal(LadoCarta(TipoCarta::Numero, c, n, rutaNumero)));
            mazo.insertarFinal(new CartaNormal(LadoCarta(TipoCarta::Numero, c, n, rutaNumero)));
        }

        // 2 veces Acciones (Roba2, Salto, Reverse)

        std::string rutaAccion = ruta.generarRuta(TipoCarta::Roba2, c);
            mazo.insertarFinal(new CartaNormal(LadoCarta(TipoCarta::Roba2, c, -1, rutaAccion)));
            mazo.insertarFinal(new CartaNormal(LadoCarta(TipoCarta::Roba2, c, -1, rutaAccion)));

            rutaAccion = ruta.generarRuta(TipoCarta::Salto, c);
            mazo.insertarFinal(new CartaNormal(LadoCarta(TipoCarta::Salto, c, -1, rutaAccion)));
            mazo.insertarFinal(new CartaNormal(LadoCarta(TipoCarta::Salto, c, -1, rutaAccion)));

            rutaAccion = ruta.generarRuta(TipoCarta::Reverse, c);
            mazo.insertarFinal(new CartaNormal(LadoCarta(TipoCarta::Reverse, c, -1, rutaAccion)));
            mazo.insertarFinal(new CartaNormal(LadoCarta(TipoCarta::Reverse, c, -1, rutaAccion)));

    }
    std::string rutaComodinColor =
        ruta.generarRuta(TipoCarta::Comodin, Color::Negro);
    std::string rutaComodinRoba4 =
        ruta.generarRuta(TipoCarta::Comodin4, Color::Negro);
    std::string rutaComodinAdivinar =
        ruta.generarRuta(TipoCarta::AdivinarCarta, Color::Negro);
    std::string rutaComodinCambiarMano =
        ruta.generarRuta(TipoCarta::CambiarMano, Color::Negro);

    // 4 Comodines Color

    for(int i=0;i<4;i++)
        mazo.insertarFinal(new CartaNormal(
            LadoCarta(TipoCarta::Comodin, Color::Negro, -1, rutaComodinColor)));

    // 4 +4

    for(int i=0;i<4;i++)
        mazo.insertarFinal(new CartaNormal(
            LadoCarta(TipoCarta::Comodin4, Color::Negro, -1, rutaComodinRoba4)));

    // 4 Adivinar

    for(int i=0;i<4;i++)
        mazo.insertarFinal(new CartaNormal(
            LadoCarta(TipoCarta::AdivinarCarta, Color::Negro, -1, rutaComodinAdivinar)));

    // 4 Cambiar Mano

    for(int i=0;i<4;i++)
        mazo.insertarFinal(new CartaNormal(
            LadoCarta(TipoCarta::CambiarMano, Color::Negro, -1, rutaComodinCambiarMano)));
}
    }


   void Juego::generarMazoFlipManual(int numMazos) {

       //* Cartas totales que debe tener un mazo de UNO FLIP
       int const CARTAS_POR_MAZO = 132;
       //* Calculamos la cantidad de cartas segun los mazos
       int cartasTotales = CARTAS_POR_MAZO * numMazos;

       //* Una carta Flip tiene dos lados
       LadoCarta* ladosClaros = new LadoCarta[cartasTotales];
       LadoCarta* ladosOscuros = new LadoCarta[cartasTotales];

       int idx = 0;

       for(int n =0; n < numMazos; n++){

       Color cClaros[] = {Color::Rojo, Color::Azul, Color::Verde, Color::Amarillo};
       Color cOscuros[] = {Color::Rosa, Color::Turquesa, Color::Naranja, Color::Purpura};


       for (int i = 0; i < 4; i++) {

           Color cLuz = cClaros[i];
           Color cDark = cOscuros[i];

           // Numero 0
           std::string rutaL0 = ruta.generarRuta(TipoCarta::Numero, cLuz, 0);
           std::string rutaD0 = ruta.generarRuta(TipoCarta::Numero, cDark, 0);

           ladosClaros[idx] = LadoCarta(TipoCarta::Numero, cLuz, 0, rutaL0);
           ladosOscuros[idx] = LadoCarta(TipoCarta::Numero, cDark, 0, rutaD0);
           idx++;

           // Números 1-9 (2 veces)
           for (int n = 1; n <= 9; n++) {

               for (int k = 0; k < 2; k++) {

                   std::string rutaL = ruta.generarRuta(TipoCarta::Numero, cLuz, n);
                   std::string rutaD = ruta.generarRuta(TipoCarta::Numero, cDark, n);

                   ladosClaros[idx] = LadoCarta(TipoCarta::Numero, cLuz, n, rutaL);
                   ladosOscuros[idx] = LadoCarta(TipoCarta::Numero, cDark, n, rutaD);
                   idx++;
               }
           }
        }

           for (int i = 0; i < 3; i++) {
               Color cLuz = cClaros[i];
               Color cDark = cOscuros[i];

               for (int j = 0; j < 2; j++) {

                   std::string rutaRoba1 = ruta.generarRuta(TipoCarta::Roba1, cLuz, -1);
                    ladosClaros[idx] = LadoCarta(TipoCarta::Roba1, cLuz, -1, rutaRoba1);

                   std::string rutaRoba3 = ruta.generarRuta(TipoCarta::Roba3, cDark, -1);
                   ladosOscuros[idx] = LadoCarta(TipoCarta::Roba3, cDark, -1, rutaRoba3);

                   idx++;
               }
               // Cambio dirección (claras y oscuras) - 2 copias cada uno
               for (int j = 0; j < 2; j++) {

                   std::string rutaReverse = ruta.generarRuta(TipoCarta::Reverse, cLuz);
                   ladosClaros[idx] = LadoCarta(TipoCarta::Reverse, cLuz, -1, rutaReverse);

                   std::string rutaReverseDark = ruta.generarRuta(TipoCarta::Reverse, cDark);
                   ladosOscuros[idx] = LadoCarta(TipoCarta::Reverse, cDark, -1, rutaReverseDark);
                   idx++;
               }

               // Bloqueo (claras) y Salto a todos (oscuras) - 2 copias cada uno
               for (int j = 0; j < 2; j++) {

                   std::string rutaSalto = ruta.generarRuta(TipoCarta::Salto, cLuz);
                   ladosClaros[idx] = LadoCarta(TipoCarta::Salto, cLuz, -1, rutaSalto);

                   std::string rutaSaltoTodos = ruta.generarRuta(TipoCarta::SaltoTodos, cDark);
                   ladosOscuros[idx] = LadoCarta(TipoCarta::SaltoTodos, cDark, -1, rutaSaltoTodos);
                   idx++;
               }

               // Flip (claras y oscuras) - 2 copias cada uno
               for(int j = 0; j < 2; j++){

                   std::string rutaFlip = ruta.generarRuta(TipoCarta::Flip, cLuz);
                   ladosClaros[idx] = LadoCarta(TipoCarta::Flip, cLuz, -1, rutaFlip);

                   std::string rutaFlipDark = ruta.generarRuta(TipoCarta::Flip, cDark);
                   ladosOscuros[idx] = LadoCarta(TipoCarta::Flip, cDark, -1, rutaFlipDark);
               }
           }

           //* Comodines especiales

           //* +2 multicolor (claras) y +6 multicolor (oscuras) - 4 copias
           std::string rutaComodinRoba2 = ruta.generarRuta(TipoCarta::Comodin, Color::Negro); // +2
           std::string rutaComodinRoba6 = ruta.generarRuta(TipoCarta::Comodin6, Color::Negro); // +6

           for (int copia = 0; copia < 4; copia++) {

               //* En el lado claro el comodin es +2
               ladosClaros[idx] = LadoCarta(TipoCarta::Comodin, Color::Negro, -1, rutaComodinRoba2);

               //* En el lado oscuro el comodin es +6
               ladosOscuros[idx] = LadoCarta(TipoCarta::Comodin6, Color::Negro, -1, rutaComodinRoba6);
               idx++;
           }

           // Cambio de color (claras) y Color eterno (oscuras) - 4 copias
           std::string rutaCambioColor = ruta.generarRuta(TipoCarta::Comodin, Color::Negro);
           std::string rutaColorEterno = ruta.generarRuta(TipoCarta::ColorEterno, Color::Negro);

           for (int copia = 0; copia < 4; copia++) {
               ladosClaros[idx] = LadoCarta(TipoCarta::Comodin, Color::Negro, -1, rutaCambioColor);
               ladosOscuros[idx] = LadoCarta(TipoCarta::ColorEterno, Color::Negro, -1, rutaColorEterno);
               idx++;
           }

           // Adivinar Carta (claras y oscuras) - 4 copias
           std::string rutaAdivinarCarta = ruta.generarRuta(TipoCarta::AdivinarCarta, Color::Negro);

           for (int copia = 0; copia < 4; copia++) {
               ladosClaros[idx] = LadoCarta(TipoCarta::AdivinarCarta, Color::Negro, -1, rutaAdivinarCarta);
               ladosOscuros[idx] = LadoCarta(TipoCarta::AdivinarCarta, Color::Negro, -1, rutaAdivinarCarta);
               idx++;
           }

           // Cambiar Mano (claras y oscuras) - 4 copias
           std::string rutaCambiarMano = ruta.generarRuta(TipoCarta::CambiarMano, Color::Negro);

           for (int copia = 0; copia < 4; copia++) {
               ladosClaros[idx] = LadoCarta(TipoCarta::CambiarMano, Color::Negro, -1, rutaCambiarMano);
               ladosOscuros[idx] = LadoCarta(TipoCarta::CambiarMano, Color::Negro, -1, rutaCambiarMano);
               idx++;
           }


       // Verificar que se generaron todas las cartas
       if (idx != cartasTotales) {
           qDebug() << "ERROR: Se generaron" << idx << "cartas, pero se esperaban" << cartasTotales;
       }

        //* Mezclar los lados oscuros para evitar repetir posicion en cada partida
       mezclarArregloLados(ladosOscuros, idx);

       //* Crear cartas Flip
       for (int i = 0; i < idx; i++) {
           Carta* nueva = new CartaFlip(ladosClaros[i], ladosOscuros[i]);
           mazo.insertarFinal(nueva);
       }

       delete[] ladosClaros;
       delete[] ladosOscuros;

       }

        qDebug() << "Mazo Flip generado con" << mazo.getSize() << "cartas (esperado:" << cartasTotales << ")";
   }




   void Juego::barajarMazo() {
       int cantidad = mazo.getSize();
       if (cantidad <= 1) return;

       Carta* tempArray[300];

       for (int i = 0; i < cantidad; i++)
           tempArray[i] = mazo.robarCarta();

       mezclarArregloCartas(tempArray, cantidad);

       for (int i = 0; i < cantidad; i++)
           mazo.insertarFinal(tempArray[i]);
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

   void Juego::aplicarEfectoCarta(Carta* cartaJugada, const std::string jugadorSeleccionado, int numeroSeleccionado, const std::string& colorSeleccionado) {

       // Obtenemos el lado activo sin importar si es CartaNormal o CartaFlip
       const LadoCarta& lado = cartaJugada->getLadoActivo();
       TipoCarta tipo = lado.getTipo();

       // Actualizamos el color activo de la mesa
       if (lado.getColor() != Color::Negro) {
           colorActivo = lado.getColor();
       }

       // Aplicamos el efecto basado en el Tipo
       switch (tipo) {
       case TipoCarta::Numero:
           // No hace nada especial, el turno avanzará normalmente al final
           break;

       case TipoCarta::Reverse:
           // Invierte el sentido (de 1 a -1, o de -1 a 1)
           sentidoJuego *= -1;

           // Regla oficial de UNO: Si solo hay 2 jugadores, Reverse actúa como Salto
           if (jugadores.getSize() == 2) {
               avanzarTurno();
           }
           break;

       case TipoCarta::Salto:
           // Avanza una vez extra para "saltar" al siguiente
           avanzarTurno();
           break;

       case TipoCarta::Roba1:{

            // El siguiente pierde el turno
           avanzarTurno();
           Jugador* victima = jugadores.obtenerElementoEnPosicion(indiceTurnoActual);

           // roba 1 carta
           Carta* robada = robarDelMazo();
           if(robada) victima ->agregarCarta(robada);
           break;
       }

       case TipoCarta::Roba2: {

           avanzarTurno();
           Jugador* victima = jugadores.obtenerElementoEnPosicion(indiceTurnoActual);

           for(int i = 0; i < 2; i++) {
               Carta* robada = robarDelMazo();
               if(robada) victima->agregarCarta(robada);
           }
           break;
       }
       case TipoCarta::Roba3:{

           avanzarTurno();
           Jugador* victima = jugadores.obtenerElementoEnPosicion(indiceTurnoActual);

           for(int i = 0; i < 3;i++){
               Carta* robada = robarDelMazo();
               if(robada) victima->agregarCarta(robada);
           }

           break;
       }

       case TipoCarta::Flip:
           // Invertimos el estado global del juego
           ladoOscuroActivo = !ladoOscuroActivo;

           // Recorremos TODAS las cartas y las volteamos
           // 1. Voltear Mazo
           for(int i = 0; i < mazo.getSize(); i++) {
               mazo.obtenerElementoEnPosicion(i)->voltear();
           }
           // 2. Voltear Descarte
           for(int i = 0; i < descarte.getSize(); i++) {
               descarte.obtenerElementoEnPosicion(i)->voltear();
           }
           // 3. Voltear Manos de Jugadores
           for(int i = 0; i < jugadores.getSize(); i++) {
               Jugador* j = jugadores.obtenerElementoEnPosicion(i);
               for(int k = 0; k < j->cantidadCartas(); k++) {
                   j->getMano().obtenerElementoEnPosicion(k)->voltear();
               }
           }
           // Actualizar el nuevo color activo según la nueva cara de la carta que quedó en el descarte
           colorActivo = cartaJugada->getLadoActivo().getColor();
           break;

       case TipoCarta::SaltoTodos:
           // damos la vuelta completa a la lista de jugadores
           for(int i = 0; i < jugadores.getSize() - 1; i++) {
               avanzarTurno();
           }
           break;

       case TipoCarta::Comodin4: {
           avanzarTurno();
           Jugador* victima = jugadores.obtenerElementoEnPosicion(indiceTurnoActual);
           for(int i = 0; i < 4; i++) {
               Carta* robada = robarDelMazo();
               if(robada) victima->agregarCarta(robada);
           }
           break;
       }

       case TipoCarta::Comodin6:{
           avanzarTurno();
           Jugador* victima = jugadores.obtenerElementoEnPosicion(indiceTurnoActual);
           for(int i=0; i<6;i++){
               Carta* robada = robarDelMazo();
               if(robada) victima->agregarCarta(robada);
           }
           break;
       }

       case TipoCarta::Comodin2:{
           avanzarTurno();
           Jugador* victima = jugadores.obtenerElementoEnPosicion(indiceTurnoActual);
           for(int i=0; i < 2; i++){
               Carta* robada = robarDelMazo();
               if(robada) victima->agregarCarta(robada);
           }
           break;
       }

       case TipoCarta::Comodin:{
           Color nuevoColor = convertirStringAColor(colorSeleccionado);
           colorActivo = nuevoColor;
           break;
       }



       case TipoCarta::AdivinarCarta:{
           if (jugadorSeleccionado.empty() || numeroSeleccionado == -1 || colorSeleccionado.empty()) {
               qDebug() << "Error: Faltan datos para adivinar carta";
               break;
           }

           Jugador* jugadorActual = getJugadorActual();
           bool adivino = adivinoCarta(jugadorSeleccionado, numeroSeleccionado, colorSeleccionado);

           if (adivino) {

               Color colorObjetivo=convertirStringAColor(colorSeleccionado);
               ListaGenerica<Carta*>& manoActual = jugadorActual->getMano();

               //* Aqui decimos que el jugador manda las cartas del mismo color a la pila de descarte

               for(int i = manoActual.getSize() -1; i >=0; i-- ){

                   Carta* carta = manoActual.obtenerElementoEnPosicion(i);
                   Color colorCarta = carta->getLadoActivo().getColor();

                   if(colorCarta == colorObjetivo){

                       Carta* cartaDescartar = manoActual.obtenerElementoEnPosicion(i);
                       manoActual.eliminarDatoEnPosicion(i);
                       descarte.insertarInicio(cartaDescartar);
                   }
               }
           } else {
               //* Decimos que el jugador que intento adivinar roba dos cartas como penalizacion
               Jugador* jugadorActual = getJugadorActual();
               for (int i = 0; i < 2; i++) {
                   if(mazo.estaVacia()) barajarDescarte();
                   Carta* robada = robarDelMazo();
                   if (robada) jugadorActual->agregarCarta(robada);
               }
           }
           break;
       }
           // ... Agrega aquí Roba1, Roba5, ColorEterno, etc. siguiendo la misma lógica

       case TipoCarta::CambiarMano:{
           int cartasEnManoVictima = 0;
           avanzarTurno();
           Jugador* victima = jugadores.obtenerElementoEnPosicion(indiceTurnoActual);
           cartasEnManoVictima = victima->getMano().getSize();

           //* Aqui decimos que el jugador manda su mano a la pila de descarte
           while(!victima->getMano().estaVacia()){
               Carta* cartaDescartar = victima->getMano().obtenerElementoEnPosicion(0);
               victima->getMano().eliminarDatoEnPosicion(0);
               descarte.insertarInicio(cartaDescartar);
           }

           //* la victima roba la misma cantidad de cartas que mando a la pila
           for(int i = 0; i < cartasEnManoVictima; i++){

               if(mazo.estaVacia()){
                   barajarDescarte();
               }
               Carta* robada = robarDelMazo();
               if(robada) victima->agregarCarta(robada);
           }

           break;
       }

       case TipoCarta::ColorEterno:{
           avanzarTurno();
           Jugador* victima = jugadores.obtenerElementoEnPosicion(indiceTurnoActual);
           Color colorRobado = Color::Negro;
           Color colorObjetivo = convertirStringAColor(colorSeleccionado);
           do{
               if(mazo.estaVacia()){
                   barajarDescarte();
               }

               Carta* robada = robarDelMazo();

               if(!robada) break;

               colorRobado = robada->getLadoActivo().getColor();
               victima->agregarCarta(robada);

           }while(colorRobado != colorObjetivo);

           break;
       }

    }

   }


   void Juego::jugarCarta(Jugador* jugador, int indiceCartaEnMano,
                          const std::string& jugadorSeleccionado,
                          int numeroAdivinado,
                          const std::string& colorAdivinado) {
       // 1. El jugador tira la carta (la quitamos de su mano)
       Carta* cartaJugada = jugador->getMano().obtenerElementoEnPosicion(indiceCartaEnMano);
       jugador->getMano().eliminarDatoEnPosicion(indiceCartaEnMano);

       // 2. La ponemos en la pila de descarte
       descarte.insertarInicio(cartaJugada);

       // 3. Procesamos qué hace esa carta
       aplicarEfectoCarta(cartaJugada, jugadorSeleccionado, numeroAdivinado, colorAdivinado);

       // 4. Preparamos el turno del siguiente jugador
       avanzarTurno();
   }

   void Juego::repartirCartas(int cartasPorJugador) {

       // Verificar que hay jugadores
       if (jugadores.estaVacia()) {
           qDebug() << "Error: No hay jugadores para repartir cartas";
           return;
       }

       // Verificar que hay suficientes cartas en el mazo
       int cartasNecesarias = jugadores.getSize() * cartasPorJugador;
       if (mazo.getSize() < cartasNecesarias) {
           qDebug() << "Error: No hay suficientes cartas en el mazo";
           return;
       }

       // Repartir cartas a cada jugador
       for (int i = 0; i < jugadores.getSize(); i++) {
           Jugador* jugadorActual = jugadores.obtenerElementoEnPosicion(i);

           for (int j = 0; j < cartasPorJugador; j++) {
               Carta* cartaARobar = robarDelMazo();
               if (cartaARobar) {
                   jugadorActual->agregarCarta(cartaARobar);
               }
           }
       }

       // Colocar la primera carta en el descarte para comenzar el juego
       if (!mazo.estaVacia()) {
           Carta* primeraCarta = robarDelMazo();
           descarte.insertarInicio(primeraCarta);

           // Actualizar el color activo según la primera carta
           if (primeraCarta) {
               colorActivo = primeraCarta->getLadoActivo().getColor();

               // Si es comodín se elige un color
               if (colorActivo == Color::Negro) {
                   // Elegir un color aleatorio
                   Color coloresNormales[] = {Color::Rojo, Color::Azul,
                                              Color::Verde, Color::Amarillo};
                   colorActivo = coloresNormales[rand() % 4];
               }
           }
       }

       qDebug() << "Reparto completado. Mazo restante:" << mazo.getSize()
                << "cartas. Primera carta en descarte.";
   }

   Jugador* Juego::getJugadorActual(){
       return jugadores.obtenerElementoEnPosicion(indiceTurnoActual);
   }

   Jugador* Juego::getJugadorSeleccionado(std::string nombreJugador){
       for(int i = 0; i < jugadores.getSize(); i++){
           Jugador* temp = jugadores.obtenerElementoEnPosicion(i);
           if(temp->getNombreJugador() == nombreJugador) return temp;
       }
       return nullptr;
   }


   bool Juego::adivinoCarta(std::string nombreSeleccionado, int numeroCarta, std::string colorCarta){
           Jugador* jugadorSeleccionado = getJugadorSeleccionado(nombreSeleccionado);

           if (!jugadorSeleccionado) return false;

           // Convertir el string color a enum Color para comparar
           Color colorAdivinado = convertirStringAColor(colorCarta);

           // Recorrer la mano del jugador seleccionado
           ListaGenerica<Carta*>& manoSeleccionada = jugadorSeleccionado->getMano();

           for (int i = 0; i < manoSeleccionada.getSize(); i++) {
               Carta* carta = manoSeleccionada.obtenerElementoEnPosicion(i);
               const LadoCarta& lado = carta->getLadoActivo();

               // Verificar si coincide número y color
               if (lado.getTipo() == TipoCarta::Numero &&
                   lado.getNumero() == numeroCarta &&
                   lado.getColor() == colorAdivinado) {
                   return true;
               }
           }

           return false;
       }


   Color Juego::convertirStringAColor(const std::string& colorStr) {
       if (colorStr == "Rojo") return Color::Rojo;
       if (colorStr == "Azul") return Color::Azul;
       if (colorStr == "Verde") return Color::Verde;
       if (colorStr == "Amarillo") return Color::Amarillo;
       // Si estás en modo oscuro
       if (colorStr == "Rosa") return Color::Rosa;
       if (colorStr == "Turquesa") return Color::Turquesa;
       if (colorStr == "Naranja") return Color::Naranja;
       if (colorStr == "Purpura") return Color::Purpura;
       // Valor por defecto
       return Color::Negro;
   }

   void Juego::barajarDescarte(){
       while(descarte.getSize()>1){
           Carta* carta = descarte.robarCarta();
           mazo.insertarFinal(carta);
       }
       barajarMazo();
   }

