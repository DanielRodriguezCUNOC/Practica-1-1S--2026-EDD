#ifndef PANTALLAJUEGO_H
#define PANTALLAJUEGO_H

#include <QWidget>
#include "juego.h"

namespace Ui {
class PantallaJuego;
}

class PantallaJuego : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaJuego(QWidget *parent = nullptr);
    ~PantallaJuego();
    void iniciarNuevaPartida(int numJugadores, bool esFlip);

private slots:
    void on_btnSalirJuego_clicked();

private:
    Ui::PantallaJuego *ui;
    //* Creamos una instancia de nuestro juego
    Juego* juego;
signals:
    void salirPartida();
};

#endif // PANTALLAJUEGO_H
