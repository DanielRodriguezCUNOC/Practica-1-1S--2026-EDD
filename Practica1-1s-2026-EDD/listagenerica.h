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
    void eliminarDatoEnPosicion(int pos);
    void barajar();

    T& obtenerPrimerElemento();
    T& obtenerElementoEnPosicion(int indice);
    T robarCarta();
    Nodo<T>* siguienteTurno(Nodo<T>* nodoActual, bool horario = true);
    bool estaVacia();
    int getSize()const;
    Nodo<T>* getCola();
    Nodo<T>* getCabeza();

};

#endif // LISTAGENERICA_H
