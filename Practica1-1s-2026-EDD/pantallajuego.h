#ifndef PANTALLAJUEGO_H
#define PANTALLAJUEGO_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "juego.h"

namespace Ui
{
    class PantallaJuego;
}

class PantallaJuego : public QWidget
{
    Q_OBJECT

private:
    Ui::PantallaJuego *ui;
    //* Creamos una instancia de nuestro juego
    Juego *juego;
    QGraphicsScene *escena;
    QGraphicsView *vista;

    int numJugadores;
    bool modoJuego;
    bool retoActivo = false;
    bool gritoUnoActivo = false;
    bool actualizandoMano = false;
signals:
    void salirPartida();
    void pedirNombres();

private slots:
    void on_btnSalirJuego_clicked();

    void on_btnRobarCarta_clicked();

    void on_btnPasarTurno_clicked();

    void on_btnRetar_clicked();
    void on_btnAvisarUNO_clicked();
    void on_btnReportar_clicked();

public:
    explicit PantallaJuego(int cantidadaJugdores, bool esFlip, bool acumulacion, bool retoMasCuatro, bool robarSinLimite, bool gritoUno, bool ganarConNegra, QWidget *parent = nullptr);
    ~PantallaJuego();
    void iniciarNuevaPartida(int numJugadores, bool esFlip);
    void dibujarMazo(ListaGenerica<Carta> mazoBackend);

public slots:
    void onCartaSeleccionada(int idCarta);
    void actualizarManoJugador();
    void actualizarTurno(QString nombreJugador);
    void actualizarDescarte(QString rutaCarta);
    void onPartidaIniciada();
    void onRobarCarta();
    void onPasarTurno();
    void onCartaInvalida();
    void onDebeJugarAntesDeRobar();
    void onMazoSinCartas();
    void onPedirColor(int indiceCarta, bool modoOscuro);
    void onPedirDatosAdivinar(int indiceCarta);
    void onRetoPosible();
    void onCartaNegraBloqueada();

private:
    void configurarConexiones();
    void mostrarCartaDescarte(Carta carta);
};

#endif // PANTALLAJUEGO_H
