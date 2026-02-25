#ifndef CONFIGURACIONES_H
#define CONFIGURACIONES_H

#include <QWidget>

namespace Ui
{
    class Configuraciones;
}

class Configuraciones : public QWidget
{
    Q_OBJECT

private:
    Ui::Configuraciones *ui;
signals:
    void volverMenuInicio();

public:
    explicit Configuraciones(QWidget *parent = nullptr);
    ~Configuraciones();
    int getCantidadJugadores() const;
    bool getEsModoFlip() const;
    bool getAcumulacion() const;
    bool getRetoMasCuatro() const;
    bool getRobarSinLimite() const;
    bool getGritoUno() const;
    bool getGanarConNegra() const;

private slots:
    void on_btnSalirConfig_clicked();
};

#endif // CONFIGURACIONES_H
