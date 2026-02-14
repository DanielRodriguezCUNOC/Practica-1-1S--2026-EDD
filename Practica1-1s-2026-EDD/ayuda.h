#ifndef AYUDA_H
#define AYUDA_H

#include <QWidget>

namespace Ui {
class Ayuda;
}

class Ayuda : public QWidget
{
    Q_OBJECT

public:
    explicit Ayuda(QWidget *parent = nullptr);
    ~Ayuda();

private slots:
    void on_btnRegresar_clicked();

private:
    Ui::Ayuda *ui;
signals:
    void volverMenuInicio();

};

#endif // AYUDA_H
