#include "mainwindow.h"
#include "configuraciones.h"
#include "ayuda.h"
#include "pantallajuego.h"
#include "botonanimado.h"
#include "ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setObjectName("MenuInicio");

    //* Inicializar todas las pantallas
    pantallaConfiguracion = new Configuraciones(this);
    pantallaAyuda = nullptr;  // Inicializar como nullptr
    pantallaJuego = nullptr;  // Inicializar como nullptr

    //* Agregar las pantallas al Stack
    ui->stackedWidget->addWidget(this->pantallaConfiguracion);

    //* Conectar botones (sin duplicados)
    connect(ui->btnJugar, &BotonAnimado::clicked, this, &MainWindow::on_btnJugar_clicked);
    connect(ui->btnConfigurar, &BotonAnimado::clicked, this, &MainWindow::on_btnConfigurar_clicked);
    connect(ui->btnAyuda, &BotonAnimado::clicked, this, &MainWindow::on_btnAyuda_clicked);
    connect(ui->btnSalir, &BotonAnimado::clicked, this, &MainWindow::on_btnSalir_clicked);

    //* NO conectar pantallaAyuda aquí porque es nullptr

    this->setAttribute(Qt::WA_StyledBackground, true);

    //* Colocar imagen de fondo
    this->setStyleSheet("QWidget#MenuInicio {"
                        "border-image: url(:/assets/VARIOS/MENU.jpg) 0 0 0 0 stretch stretch;"
                        "background-position: center;"
                        "}");
}

void MainWindow::on_btnAyuda_clicked()
{
    if(!pantallaAyuda){
        pantallaAyuda = new Ayuda(this);
        ui->stackedWidget->addWidget(pantallaAyuda);

        //* AQUÍ conectar la señal después de crear pantallaAyuda
        connect(pantallaAyuda, &Ayuda::volverMenuInicio, this, [=](){
            ui->stackedWidget->setCurrentIndex(0);
        });
    }
    ui->stackedWidget->setCurrentWidget(pantallaAyuda);
}

void MainWindow::on_btnJugar_clicked()
{
    if(!pantallaJuego){
        pantallaJuego = new PantallaJuego(this);
        ui->stackedWidget->addWidget(pantallaJuego);
    }
    ui->stackedWidget->setCurrentWidget(pantallaJuego);
}

void MainWindow::on_btnConfigurar_clicked()
{
    ui->stackedWidget->setCurrentWidget(pantallaConfiguracion);
}

void MainWindow::on_btnSalir_clicked()
{
    //* Mostrar confirmacion
    QMessageBox::StandardButton respuesta = QMessageBox::question( this, "UNO",
                                                                  "¿Estás seguro de que quieres salir?",
                                                                  QMessageBox::No | QMessageBox::Yes,
                                                                  QMessageBox::Yes
                                                                  );
    if (respuesta == QMessageBox::Yes) {
        qApp->quit();
    }
}

MainWindow::~MainWindow(){
    delete ui;
}
