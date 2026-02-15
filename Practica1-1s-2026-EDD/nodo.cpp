#include "nodo.h"

template <typename T>
Nodo<T>::Nodo(const T& dato): dato(dato), siguiente(nullptr), anterior(nullptr) {}

template <typename T>
Nodo<T>::~Nodo(){}

template <typename T>
Nodo<T>* Nodo<T>::getSiguiente() const{
    return siguiente;
}

template <typename T>
 Nodo<T>* Nodo<T>::getAnterior() const {
    return anterior;
}
template <typename T>
void Nodo<T>::setSiguiente(Nodo<T> dato){
    this->dato = dato;
}
template <typename T>
void Nodo<T>::setAnterior(Nodo<T> dato){
    return siguiente;
}

template <typename T>
T& Nodo<T>::getDato(){
    return dato;
}
