
#include "listagenerica.h"
#include "carta.h"
#include "jugador.h"
#include "excepciones.h"
#include "qdebug.h"

template class ListaGenerica<Carta>;
template class ListaGenerica<Jugador*>;
template <typename T>
ListaGenerica<T>::ListaGenerica():
    cabeza(nullptr), cola(nullptr), size(0){}


template <typename T>
ListaGenerica<T>::~ListaGenerica(){

    if (estaVacia()) return;

    Nodo<T>* actual = cabeza;

    do {
        Nodo<T>* siguiente = actual->getSiguiente();
        delete actual;
        actual = siguiente;
    } while (actual != cabeza);

    cabeza = nullptr;
    cola = nullptr;
    size = 0;
}

template <typename T>
void ListaGenerica<T>::insertarInicio(const T& dato) {

    //* Se crea un nuevo nodo
    Nodo<T>* nuevo = new Nodo<T>(dato);

    //* Verificamos si esta vacia la lista
    if (estaVacia()) {
        //Almacenamos el nuevo nodo en la cabeza de la lista
        this->cabeza = nuevo;
        //Almacenar el nuevo nodo en la cola de la lista
        this->cola = nuevo;
        //Hacemos que el siguiente apunte al nuevo nodo
        nuevo->setSiguiente(nuevo);
        //Hacemos que el anterior apunte al nuevo nodo
        nuevo->setAnterior(nuevo);
        size++;
        return;
    }
    //* Si no esta vacia

    //Obtenemos el ultimo nodo de la lista
    Nodo<T>* cola = this->getCola();
    //El nodo siguiente del nuevo nodo apunta al nodo cabeza (primer elemento)
    nuevo->setSiguiente(this->cabeza);
    //El nodo anterior del nuevo nodo apunta al nodo cola (ultimo elemento)
    nuevo->setAnterior(this->cola);
    //* Ahora el nodo anterior de la cabeza apunta al nuevo nodo
    this->cabeza->setAnterior(nuevo);
    //* El nodo siguiente de la cola apunta al nuevo nodo
    cola ->setSiguiente(nuevo);
    //* Ahora cabeza sera la direccion en memoria del nuevo nodo
    this->cabeza = nuevo;

    size++;
}

template <typename T>
void ListaGenerica<T>::insertarFinal(const T& dato) {

    if (estaVacia()) {
        insertarInicio(dato);
        return;
    }

    Nodo<T>* nuevo = new Nodo<T>(dato);
    Nodo<T>* cola = cabeza->getAnterior();

    nuevo->setSiguiente(cabeza);
    nuevo->setAnterior(cola);

    cola->setSiguiente(nuevo);
    cabeza->setAnterior(nuevo);

    this->cola = nuevo;

    size++;
}

template <typename T>
void ListaGenerica<T>::eliminarDatoEnPosicion(int pos){

    if(estaVacia()) return;

    if(pos < 0 || pos >= size) return;

    Nodo<T>* actual = cabeza;

    for(int i = 0; i < pos; i++){
        actual = actual->getSiguiente();
    }

    if(size == 1){
        delete actual;
        cabeza = nullptr;
        cola = nullptr;
    }
    else{
        Nodo<T>* next = actual->getSiguiente();
        Nodo<T>* prev = actual->getAnterior();

        prev->setSiguiente(next);
        next->setAnterior(prev);

        if(actual == cabeza) cabeza = next;
        if(actual == cola)   cola = prev;

        delete actual;
    }

    size--;
}

template <typename T>
bool ListaGenerica<T>::estaVacia(){
    return this->cabeza == nullptr;
}

template <typename T>
int ListaGenerica<T>::getSize()const{

    return size;
}

template <typename T>
Nodo<T>* ListaGenerica<T>::getCola(){
    return cola;
}

template <typename T>
T& ListaGenerica<T>::obtenerPrimerElemento(){
    return cabeza->getDato();
}

template <typename T>
T& ListaGenerica<T>::obtenerElementoEnPosicion(int indice){

    if (indice < 0 || indice >= size)
        throw IndiceFueraDeRangoException();

    Nodo<T>* actual = cabeza;

    for (int i = 0; i < indice; i++)
        actual = actual->getSiguiente();

    return actual->getDato();
}

template <typename T>
T ListaGenerica<T>::robarCarta(){

    if (estaVacia())
        throw IndiceFueraDeRangoException();

    T dato = cabeza->getDato();
    eliminarDatoEnPosicion(0);
    return dato;
}

template <typename T>
void ListaGenerica<T>::barajar() {

    if (size <= 1) return;

    // 1. Creamos un arreglo temporal de PUNTEROS A NODO (No a datos, así evitamos el truene)
    Nodo<T>** arregloNodos = new Nodo<T>*[size];
    Nodo<T>* actual = cabeza;

    // 2. Guardamos las direcciones de todos los nodos
    for (int i = 0; i < size; i++) {
        arregloNodos[i] = actual;
        actual = actual->getSiguiente();
    }

    // 3. Mezclamos las posiciones de los Nodos (Fisher-Yates)
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Nodo<T>* temp = arregloNodos[i];
        arregloNodos[i] = arregloNodos[j];
        arregloNodos[j] = temp;
    }

    // 4. Reconstruimos los enlaces de la lista circular con el nuevo orden
    for (int i = 0; i < size; i++) {
        Nodo<T>* nodoActual = arregloNodos[i];
        Nodo<T>* nodoPrevio = (i == 0) ? arregloNodos[size - 1] : arregloNodos[i - 1];
        Nodo<T>* nodoSiguiente = (i == size - 1) ? arregloNodos[0] : arregloNodos[i + 1];

        nodoActual->setAnterior(nodoPrevio);
        nodoActual->setSiguiente(nodoSiguiente);
    }

    // 5. Actualizamos quién es la cabeza y la cola
    cabeza = arregloNodos[0];
    cola = arregloNodos[size - 1];

    // Limpiamos nuestra memoria temporal
    delete[] arregloNodos;
}

template <typename T>
Nodo<T>* ListaGenerica<T>::siguienteTurno(Nodo<T>* nodoActual, bool horario){
    return horario ? nodoActual->getSiguiente()
                   : nodoActual->getAnterior();
}

template <typename T>
Nodo<T>* ListaGenerica<T>::getCabeza(){
    return this->cabeza;
}
