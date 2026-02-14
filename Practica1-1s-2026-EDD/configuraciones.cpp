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
