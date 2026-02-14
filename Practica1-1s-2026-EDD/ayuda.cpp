#include "ayuda.h"
#include "botonanimado.h"
#include "ui_ayuda.h"

Ayuda::Ayuda(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Ayuda)
{
    ui->setupUi(this);

    //* Regresamos al Menu de Inicio
    connect(ui->btnRegresar, &BotonAnimado::clicked, this, &Ayuda::volverMenuInicio);
}

Ayuda::~Ayuda()
{
    delete ui;
}

void Ayuda::on_btnRegresar_clicked()
{

}

