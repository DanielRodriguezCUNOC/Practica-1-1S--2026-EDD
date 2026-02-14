#include "botonanimado.h"

BotonAnimado::BotonAnimado(QWidget *parent) : QPushButton(parent) {
    animacion = new QPropertyAnimation(this, "geometry");
    animacion->setDuration(200); // 200 milisegundos
    animacion->setEasingCurve(QEasingCurve::OutBack); // Efecto de "pequeño rebote"
}

void BotonAnimado::enterEvent(QEnterEvent *event) {

    if(!inicializado){
        sizeOriginal = geometry();
        inicializado = true;
    }

    // Lo movemos 5px a la izquierda/arriba y lo agrandamos 10px
    QRect rectFinal = QRect(sizeOriginal.x() - 5, sizeOriginal.y() - 5,
                            sizeOriginal.width() + 10, sizeOriginal.height() + 10);

    animacion->stop();
    animacion->setEndValue(rectFinal);
    animacion->start();

    QPushButton::enterEvent(event);
}

void BotonAnimado::leaveEvent(QEvent *event) {
    animacion->stop();
    animacion->setEndValue(sizeOriginal);
    animacion->start();

    QPushButton::leaveEvent(event);
}
BotonAnimado::~BotonAnimado(){}
