#include "configuraciones.h"
#include "ui_configuraciones.h"

Configuraciones::Configuraciones(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Configuraciones)
{
    ui->setupUi(this);
}

Configuraciones::~Configuraciones()
{
    delete ui;
}

int Configuraciones::getCantidadJugadores() const{
    return ui->sbNumJugadores->value();
}

bool Configuraciones::getEsModoFlip() const{
    return ui->cbModoFlip->isChecked();
}
