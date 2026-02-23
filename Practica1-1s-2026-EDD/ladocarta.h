#ifndef LADOCARTA_H
#define LADOCARTA_H
#include <string>
#include "utils.h"

class Juego;

class LadoCarta
{
private:
    TipoCarta tipo;
    Color color;
    int numero;
    std::string rutaArchivo;
public:
    // Constructor n=-1 sirve como identificador para las cartas que tienen efecto
    LadoCarta(TipoCarta t, Color c, int n, std::string ruta);

    LadoCarta() = default;
    virtual LadoCarta* clonar() const=0;

    //* Metodos y funciones que seran polimorficos
    virtual ~LadoCarta() = default;
    virtual void aplicarEfecto(Juego* juego, const std::string& colorSeleccionado ="", const std::string jugadorSeleccionado = "", int numeroSeleccionado = -1) = 0;

    //* Metodos comunes
    TipoCarta getTipo() const;
    Color getColor() const;
    int getNumero() const;

    // Devuelve la ruta de la imagen
    std::string getRutaArchivo() const;
};

#endif // LADOCARTA_H
