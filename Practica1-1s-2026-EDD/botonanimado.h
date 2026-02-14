#ifndef BOTONANIMADO_H
#define BOTONANIMADO_H
#include <QPushButton>
#include <QPropertyAnimation>
#include <QEasingCurve>

class BotonAnimado: public QPushButton
{
    Q_OBJECT
public:
    explicit BotonAnimado(QWidget *parent = nullptr);
    ~BotonAnimado();

protected:
    //* Analizar comportamiento del mouse
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QPropertyAnimation *animacion;
    QRect sizeOriginal;
    bool inicializado=false;
    void configurarAnimacion(QRect finalRect);
};

#endif // BOTONANIMADO_H
