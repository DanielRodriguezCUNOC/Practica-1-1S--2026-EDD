#include "listagenerica.h"

template <typename T>
ListaGenerica<T>::ListaGenerica():
    cabeza(nullptr), cola(nullptr), size(0){}


template <typename T>
ListaGenerica<T>::~ListaGenerica(){
    Nodo<T>* actual = cabeza;

    while (actual != nullptr) {
        //* Almacenar la referencia a la que apunta el nodo siguiente
        Nodo<T>* siguiente = actual->getSiguiente();

        delete actual;
        actual = siguiente;
    }
}

template <typename T>
void ListaGenerica<T>::insertarInicio(const T& dato) {
    Nodo<T>* nuevo = new Nodo<T>(dato);

    if (estaVacia()) {
        cabeza = cola = nuevo;
    } else {
        nuevo->setSiguiente(cabeza);
        cabeza->setAnterior(nuevo);
        cabeza = nuevo;
    }
    size++;
}

template <typename T>
void ListaGenerica<T>::insertarFinal(const T& dato) {
    Nodo<T>* nuevo = new Nodo<T>(dato);

    if (estaVacia()) {
        cabeza = cola = nuevo;
    } else {
        nuevo->setAnterior(cola);
        cola->setSiguiente(nuevo);
        cola = nuevo;
    }
    size++;
}

template <typename T>
void ListaGenerica<T>::eliminarDato(const T& dato) {
    Nodo<T>* actual = cabeza;

    while (actual != nullptr) {
        if (actual->getDato() == dato) {
            // Ajustar punteros
            if (actual->getAnterior() != nullptr) {
                actual->getAnterior()->setSiguiente(actual->getSiguiente());
            } else {
                cabeza = actual->getSiguiente();  // Era la cabeza
            }

            if (actual->getSiguiente() != nullptr) {
                actual->getSiguiente()->setAnterior(actual->getAnterior());
            } else {
                cola = actual->getAnterior();  // Era la cola
            }

            delete actual;
            size--;
            return;
        }
        actual = actual->getSiguiente();
    }
}

template <typename T>
bool ListaGenerica<T>::estaVacia(){
    return cabeza == nullptr;
}

template <typename T>
int ListaGenerica<T>::getSize(){
    return size;
}
