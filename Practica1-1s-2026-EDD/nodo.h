#ifndef NODO_H
#define NODO_H


template <typename T>
class Nodo
{

private:
    T dato;
    Nodo<T>* siguiente;
    Nodo<T>* anterior;

public:
    Nodo(const T& dato);
    ~Nodo();
    Nodo<T>* getSiguiente() const;
    Nodo<T>* getAnterior() const;
    T& getDato();
    void setSiguiente(Nodo<T>* dato);
    void setAnterior(Nodo<T>* dato);
};

#include "nodo.tpp"

#endif // NODO_H
