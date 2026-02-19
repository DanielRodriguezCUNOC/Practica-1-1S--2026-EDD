#ifndef JUEGO_H
#define JUEGO_H
#include "carta.h"
#include "listagenerica.h"
#include "jugador.h"
#include "rutaimagenes.h"
#include "utils.h"

class Juego
{
private:
    // Estructuras de datos principales
    ListaGenerica<Carta*> mazo;
    ListaGenerica<Carta*> descarte;
    ListaGenerica<Jugador*> jugadores;

    bool ladoOscuroActivo;

    // --- MÉTODOS PRIVADOS AUXILIARES (Gestión de Memoria Manual) ---

    // Genera cartas simples
    void generarMazoNormalManual(int numMazos);

    // Genera cartas dobles con reverso aleatorio
    void generarMazoFlipManual(int numMazos);

    // Algoritmos manuales de mezcla
    void mezclarArregloLados(LadoCarta* arreglo, int tamano);
    void mezclarArregloCartas(Carta** arreglo, int tamano);

public:
    Juego();
    ~Juego();

    // Inicializa el juego según el modo elegido
    void inicializarMazo(bool modoFlip, int cantidadJugadores);

    // Baraja las cartas que ya están en la lista 'mazo'
    void barajarMazo();

    // Saca una carta del mazo
    Carta* robarDelMazo();

    bool esLadoOscuro() const;
    void setLadoOscuro(bool estado);

    int getCantidadCartasMazo();

    RutaImagenes ruta;

};

#endif // JUEGO_H
