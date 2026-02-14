#include "pantallajuego.h"
#include "ui_pantallajuego.h"

PantallaJuego::PantallaJuego(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaJuego)
{
    ui->setupUi(this);
    this ->setObjectName("PantallaJuego");

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
    delete ui;
}
