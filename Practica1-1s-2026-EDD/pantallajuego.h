#ifndef PANTALLAJUEGO_H
#define PANTALLAJUEGO_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "juego.h"

namespace Ui {
class PantallaJuego;
}

class PantallaJuego : public QWidget
{
    Q_OBJECT

private:
    Ui::PantallaJuego *ui;
    //* Creamos una instancia de nuestro juego
    Juego* juego;
    QGraphicsScene* escena;
    QGraphicsView* vista;

    int numJugadores;
    bool modoJuego;
signals:
    void salirPartida();
    void pedirNombres();

private slots:
    void on_btnSalirJuego_clicked();

public:
    explicit PantallaJuego(int cantidadaJugdores, bool esFlip, QWidget *parent = nullptr);
    ~PantallaJuego();
    void iniciarNuevaPartida(int numJugadores, bool esFlip);
    void dibujarMazo(ListaGenerica<Carta> mazoBackend);
    void onCartaSeleccionada(int id);



};

#endif // PANTALLAJUEGO_H
