#ifndef CONFIGURACIONES_H
#define CONFIGURACIONES_H

#include <QWidget>

namespace Ui {
class Configuraciones;
}

class Configuraciones : public QWidget
{
    Q_OBJECT

public:
    explicit Configuraciones(QWidget *parent = nullptr);
    ~Configuraciones();

private:
    Ui::Configuraciones *ui;
};

#endif // CONFIGURACIONES_H
