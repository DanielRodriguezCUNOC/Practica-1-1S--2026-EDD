#ifndef UTILS_H
#define UTILS_H

enum class TipoCarta {
    // Normal / Lado Claro
    Numero, Roba1, Roba2, Salto, Reverse, Comodin,
    // Lado Oscuro (Flip)
    Roba4, Roba5, SaltoTodos, ColorEterno, Flip,
    // Especial para el reverso del UNO Normal
    DorsoGenerico,
    //Cartas personalizadas
    AdivinarCarta, CambiarMano
};

enum class Color {
    Rojo, Amarillo, Azul, Verde,
    Naranja, Rosa, Turquesa, Violeta,
    // Especiales
    Negro,      // Comodines
    Indefinido  // Para el reverso genérico
};

#endif // UTILS_H
