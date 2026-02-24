#include "configuraciones.h"
#include "ui_configuraciones.h"

Configuraciones::Configuraciones(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Configuraciones)
{
    ui->setupUi(this);

    connect(ui->btnSalirConfig, &BotonAnimado::clicked, this, &Configuraciones::volverMenuInicio);
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

void Configuraciones::on_btnSalirConfig_clicked() {}
