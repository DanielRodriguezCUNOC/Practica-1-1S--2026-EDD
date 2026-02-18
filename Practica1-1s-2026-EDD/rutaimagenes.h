#ifndef RUTAIMAGENES_H
#define RUTAIMAGENES_H
#include <string>
#include "utils.h"
class RutaImagenes
{
public:
    RutaImagenes();
    std::string generarRuta(TipoCarta tipoCarta, Color color, int numero = - 1);

private:
    std::string obtenerCarpetaColor(Color color);
    std::string obtenerNombreArchivo(TipoCarta tipo,Color color, int numero, const std::string& modo);
    std::string obtenerPrefijoColor(Color color);
    std::string modoDeJuego(Color color);
    bool esColorClaro(Color color);
};

#endif // RUTAIMAGENES_H
