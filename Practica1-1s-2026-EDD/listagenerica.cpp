
#include "listagenerica.h"
template <typename T>
ListaGenerica<T>::ListaGenerica():
    cabeza(nullptr), cola(nullptr), size(0){}


template <typename T>
ListaGenerica<T>::~ListaGenerica(){

    if(estaVacia()) return;

    Nodo<T>* actual = cabeza;

    do{
        Nodo<T>* siguiente = actual->getSiguiente();
        delete actual;
        actual = siguiente;

    }while(actual != cabeza);
    cabeza = nullptr;
    cola = nullptr;
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

    cola = nuevo;

    size++;
}

template <typename T>
void ListaGenerica<T>::eliminarDato(const T& dato) {

    if(estaVacia()) return;

    Nodo<T>* actual = cabeza;

    for (int i = 0; i<size; i++) {
        if(actual->getDato()==dato){

            if(size == 1){
                delete actual;
                cabeza = nullptr;
                cola = nullptr;
            }else{
                Nodo<T>* next = actual->getSiguiente();
                Nodo<T>* prev = actual->getAnterior();

                prev->setSiguiente(next);
                next->setAnterior(prev);

                if(actual == cabeza) cabeza = next;
                if(actual == cola) cola = prev;

                delete actual;
            }
            size--;
            return;
        }
        actual = actual->getSiguiente();
    }
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
    if(this->estaVacia()) return nullptr;

    Nodo<T>* actual = this->cabeza;

    while(actual->getSiguiente() != this->cabeza){
        actual = actual->getSiguiente();
    }
    return actual;
}

template <typename T>
T& ListaGenerica<T>::obtenerPrimerElemento(){
    return cabeza->getDato();
}

template <typename T>
T ListaGenerica<T>::robarCarta(){
    T dato = cabeza->getDato();
    eliminarDatoEnPosicion(0);
    return dato;
}

template <typename T>
Nodo<T>* ListaGenerica<T>::siguienteTurno(Nodo<T>* nodoActual, bool horario){
    return horario ? nodoActual->getSiguiente()
                   : nodoActual->getAnterior();
}
