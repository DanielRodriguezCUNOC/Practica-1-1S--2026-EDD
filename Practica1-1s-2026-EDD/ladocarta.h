#ifndef LADOCARTA_H
#define LADOCARTA_H
#include <string>
#include "utils.h"


class LadoCarta
{
private:
    TipoCarta tipo;
    Color color;
    int numero;
    std::string rutaArchivo;

    // Métodos privados para generar nombres de archivo
    std::string colorToString() const;
    std::string tipoToString() const;

public:
    // Constructor n=-1 sirve como identificador para las cartas que tienen efecto
    LadoCarta(TipoCarta t, Color c, int n, std::string ruta);
    LadoCarta() = default;

    TipoCarta getTipo() const;
    Color getColor() const;
    int getNumero() const;


    std::string getNombreCarpetaColor() const;

    std::string getNombreArchivo() const;

    // Devuelve la ruta de la imagen
    std::string getRutaArchivo() const;
};

#endif // LADOCARTA_H
