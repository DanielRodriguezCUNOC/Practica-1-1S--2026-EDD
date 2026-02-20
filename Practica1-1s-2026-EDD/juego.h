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

    //* Para saber si esta del lado del flip
    bool ladoOscuroActivo;
    //* Numero del jugador
    int indiceTurnoActual;
    //* 1 sentido horario, -1 sentido antihorario
    int sentidoJuego;
    //* El color que esta en la pila de descarte
    Color colorActivo;
    //* Permite obtener lass rutas de las imagenes
    RutaImagenes ruta;

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

    void avanzarTurno();
    //* Metodo para la carta Reverse
    void retrocederTurno();

    //* Metodo para aplicar los efectos
    void jugarCarta(Jugador* jugador, int indiceCartaEnMano);
    void aplicarEfectoCarta(Carta* cartaJugada);
};

#endif // JUEGO_H
