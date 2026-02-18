#ifndef UTILS_H
#define UTILS_H

enum class TipoCarta {
    // Normal / Lado Claro
    Numero, Roba1, Roba2, Salto, Reverse, Comodin, Comodin4,
    // Lado Oscuro (Flip)
    Roba3, Comodin6, Comodin2, SaltoTodos, ColorEterno, CambiarDireccion, Flip,
    // Especial para el reverso del UNO Normal
    DorsoGenerico,
    //Cartas personalizadas
    AdivinarCarta, CambiarMano
};

enum class Color {
    Rojo, Amarillo, Azul, Verde,
    Naranja, Rosa, Turquesa, Purpura,
    // Especiales
    Negro,      // Comodines
    Indefinido  // Para el reverso genérico
};

#endif // UTILS_H
