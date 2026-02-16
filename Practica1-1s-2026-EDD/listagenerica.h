#ifndef LISTAGENERICA_H
#define LISTAGENERICA_H
#include "nodo.h"

template <typename T>
class ListaGenerica
{
private:
    Nodo<T>* cabeza;
    Nodo<T>* cola;
    int size;

public:
    ListaGenerica();
    ~ListaGenerica();
    void insertarInicio(const T& dato);
    void insertarFinal(const T& dato);
    void eliminarDato(const T& dato);
    void eliminarDatoEnPosicion(int pos);
    T& obtenerPrimerElemento();
    T robarCarta();
    Nodo<T>* siguienteTurno(Nodo<T>* nodoActual, bool horario = true);
    bool estaVacia();
    int getSize()const;
    Nodo<T>* getCola();
};

#endif // LISTAGENERICA_H
