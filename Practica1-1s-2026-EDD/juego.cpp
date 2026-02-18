#include "juego.h"
#include "cartanormal.h"
#include "cartaflip.h"
Juego::Juego(): ladoOscuroActivo(false){
}

Juego::~Juego(){}

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

void Juego::inicializarMazo(bool modoFlip){


    while(!mazo.estaVacia()){
        Carta* c = mazo.robarCarta();
        delete c;
    }
    ladoOscuroActivo = false;

    modoFlip ? generarMazoFlipManual() : generarMazoNormalManual();

    barajarMazo();
}

void Juego::mezclarArregloLados(LadoCarta* arreglo, int size){
    for (int i = 0; i > size-1; i++) {
        int j = rand() % (i+1);
        LadoCarta temporal = arreglo[i];
        arreglo[i] = arreglo[j];
        arreglo[j] = temporal;
    }
}

void Juego::mezclarArregloCartas(Carta** arreglo, int size){
    for(int i = 0; i>size -1; i++){
        int j = rand() % (i+1);
        Carta* temporal = arreglo[i];
        arreglo[i] = arreglo[j];
        arreglo[j] = temporal;
    }
}

void Juego::generarMazoNormalManual(){
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

    // 4 Comodines Color
    std::string rutaComodinColor =
        ruta.generarRuta(TipoCarta::Comodin, Color::Negro);

    for(int i=0;i<4;i++)
        mazo.insertarFinal(new CartaNormal(
            LadoCarta(TipoCarta::Comodin, Color::Negro, -1, rutaComodinColor)));


    // 4 +4
    std::string rutaComodinRoba4 =
        ruta.generarRuta(TipoCarta::Comodin4, Color::Negro);

    for(int i=0;i<4;i++)
        mazo.insertarFinal(new CartaNormal(
            LadoCarta(TipoCarta::Comodin4, Color::Negro, -1, rutaComodinRoba4)));


    // 4 Adivinar
    std::string rutaComodinAdivinar =
        ruta.generarRuta(TipoCarta::AdivinarCarta, Color::Negro);

    for(int i=0;i<4;i++)
        mazo.insertarFinal(new CartaNormal(
            LadoCarta(TipoCarta::AdivinarCarta, Color::Negro, -1, rutaComodinAdivinar)));


    // 4 Cambiar Mano
    std::string rutaComodinCambiarMano =
        ruta.generarRuta(TipoCarta::CambiarMano, Color::Negro);

    for(int i=0;i<4;i++)
        mazo.insertarFinal(new CartaNormal(
            LadoCarta(TipoCarta::CambiarMano, Color::Negro, -1, rutaComodinCambiarMano)));
   }


   void Juego::generarMazoFlipManual() {

       const int MAX_CAPACIDAD = 120;

       LadoCarta* ladosClaros = new LadoCarta[MAX_CAPACIDAD];
       LadoCarta* ladosOscuros = new LadoCarta[MAX_CAPACIDAD];

       int idx = 0;

       Color cClaros[] = {Color::Rojo, Color::Azul, Color::Verde, Color::Amarillo};
       Color cOscuros[] = {Color::Rosa, Color::Turquesa, Color::Naranja, Color::Purpura};

       for (int i = 0; i < 4; i++) {

           Color cLuz = cClaros[i];
           Color cDark = cOscuros[i];

           // 1. Números 1-9 (2 veces)
           for (int n = 1; n <= 9; n++) {
               for (int k = 0; k < 2; k++) {

                   std::string rutaL = ruta.generarRuta(TipoCarta::Numero, cLuz, n);
                   std::string rutaD = ruta.generarRuta(TipoCarta::Numero, cDark, n);

                   ladosClaros[idx] = LadoCarta(TipoCarta::Numero, cLuz, n, rutaL);
                   ladosOscuros[idx] = LadoCarta(TipoCarta::Numero, cDark, n, rutaD);
                   idx++;
               }
           }

           // 2. Acciones
           TipoCarta actLuz[]  = {TipoCarta::Roba1, TipoCarta::Salto, TipoCarta::Reverse};
           TipoCarta actDark[] = {TipoCarta::Roba3, TipoCarta::SaltoTodos, TipoCarta::Reverse};

           for (int a = 0; a < 3; a++) {
               for (int k = 0; k < 2; k++) {

                   std::string rutaL = ruta.generarRuta(actLuz[a], cLuz, -1);
                   std::string rutaD = ruta.generarRuta(actDark[a], cDark, -1);

                   ladosClaros[idx] = LadoCarta(actLuz[a], cLuz, -1, rutaL);
                   ladosOscuros[idx] = LadoCarta(actDark[a], cDark, -1, rutaD);
                   idx++;
               }
           }

           // 3. Flip (2)
           for (int k = 0; k < 2; k++) {

               std::string rutaL = ruta.generarRuta(TipoCarta::Flip, cLuz, -1);
               std::string rutaD = ruta.generarRuta(TipoCarta::Flip, cDark, -1);

               ladosClaros[idx] = LadoCarta(TipoCarta::Flip, cLuz, -1, rutaL);
               ladosOscuros[idx] = LadoCarta(TipoCarta::Flip, cDark, -1, rutaD);
               idx++;
           }

           // 4. Comodines
           {
               std::string rutaL = ruta.generarRuta(TipoCarta::Comodin, Color::Negro, -1);
               std::string rutaD = ruta.generarRuta(TipoCarta::ColorEterno, Color::Negro, -1);

               ladosClaros[idx] = LadoCarta(TipoCarta::Comodin, Color::Negro, -1, rutaL);
               ladosOscuros[idx] = LadoCarta(TipoCarta::ColorEterno, Color::Negro, -1, rutaD);
               idx++;

           }

           // 5. Cartas personalizdas

           for (int k = 0; k < 4; k++) {

               std::string rutaL = ruta.generarRuta(TipoCarta::AdivinarCarta, Color::Negro, -1);
               std::string rutaD = ruta.generarRuta(TipoCarta::CambiarMano, Color::Negro, -1);

               ladosClaros[idx] = LadoCarta(TipoCarta::AdivinarCarta, Color::Negro, -1, rutaL);
               ladosOscuros[idx] = LadoCarta(TipoCarta::CambiarMano, Color::Negro, -1, rutaD);
               idx++;
           }
       }

       mezclarArregloLados(ladosOscuros, idx);

       for (int i = 0; i < idx; i++) {
           Carta* nueva = new CartaFlip(ladosClaros[i], ladosOscuros[i]);
           mazo.insertarFinal(nueva);
       }

       delete[] ladosClaros;
       delete[] ladosOscuros;
   }




void Juego::barajarMazo() {
    int cantidad = mazo.getSize();
    if (cantidad <= 1) return;

    // 1. Crear arreglo temporal de punteros
    Carta** tempArray = new Carta*[cantidad];

    // 2. Vaciar la lista al arreglo
    for (int i = 0; i < cantidad; i++) {
        tempArray[i] = mazo.robarCarta(); // Saca el nodo y devuelve el dato
    }

    // 3. Mezclar el arreglo de punteros
    mezclarArregloCartas(tempArray, cantidad);

    // 4. Rellenar la lista de nuevo
    for (int i = 0; i < cantidad; i++) {
        mazo.insertarFinal(tempArray[i]);
    }

    // 5. Borrar el arreglo auxiliar (no las cartas, solo el contenedor de punteros)
    delete[] tempArray;
}
