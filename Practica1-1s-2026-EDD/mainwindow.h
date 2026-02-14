#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "configuraciones.h"
#include "ayuda.h"
#include "pantallajuego.h"
#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Configuraciones *pantallaConfiguracion;
    Ayuda *pantallaAyuda;
    PantallaJuego *pantallaJuego;

private slots:
    void on_btnJugar_clicked();
    void on_btnConfigurar_clicked();
    void on_btnAyuda_clicked();
    void on_btnSalir_clicked();
};
#endif // MAINWINDOW_H
