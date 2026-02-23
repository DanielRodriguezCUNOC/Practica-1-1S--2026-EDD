#ifndef UTILS_H
#define UTILS_H

enum class TipoCarta {
    // Normal / Lado Claro
    NUMERO, ROBA1, ROBA2, SALTO, REVERSE, COMODIN, COMODIN4, BLOQUEO,
    // Lado Oscuro (Flip)
    ROBA3, COMODIN6, COMODIN2, SALTODOS, COLORETERNO, CAMBIARDIRECCION, FLIP,
    // Especial para el reverso del UNO Normal
    DORSOGENERICO,
    // Cartas personalizadas
    ADIVINARCARTA, CAMBIARMANO
};

enum class Color {
    ROJO, AMARILLO, AZUL, VERDE,
    NARANJA, ROSA, TURQUESA, PURPURA,
    // Especiales
    NEGRO,      // Comodines
    INDEFINIDO  // Para el reverso genérico
};

#endif // UTILS_H
