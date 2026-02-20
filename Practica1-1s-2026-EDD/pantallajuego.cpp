#include "pantallajuego.h"
#include "botonanimado.h"
#include "ui_pantallajuego.h"

PantallaJuego::PantallaJuego(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaJuego)
    //* Inicializar el puntero de juego
    , juego(new Juego())
{
    ui->setupUi(this);
    this ->setObjectName("PantallaJuego");
    connect(ui->btnSalirJuego, &BotonAnimado::clicked, this, &PantallaJuego::salirPartida);

    this->setAttribute(Qt::WA_StyledBackground, true);
    //* Colocar imagen de fondo
    this->setStyleSheet("QWidget#PantallaJuego {"
                        "border-image: url(:/assets/VARIOS/Board.png) 0 0 0 0 stretch stretch;"
                        "background-repeat: no-repeat;"
                        "background-position: center;"
                        "margin: 0px;"
                        "padding: 0px;"
                        "}");
}

PantallaJuego::~PantallaJuego()
{
    delete juego;
    delete ui;
}
void PantallaJuego:: iniciarNuevaPartida(int numJugadores, bool esFlip){
    //* Inicializar el mazo segun el modo
    juego->inicializarMazo(esFlip, numJugadores);

}
void PantallaJuego::on_btnSalirJuego_clicked() {}
