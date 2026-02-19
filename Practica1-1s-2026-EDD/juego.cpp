#include "juego.h"
#include "cartanormal.h"
#include "cartaflip.h"
#include <QWidget>
Juego::Juego(): ladoOscuroActivo(false){
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

