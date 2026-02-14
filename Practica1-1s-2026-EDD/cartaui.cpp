#include "cartaui.h"
#include <QPixmap>
#include <QCursor>

CartaUi::CartaUi(int posicionCarta, QString rutaImg, QGraphicsItem *parent):
    QObject(), QGraphicsPixmapItem(parent), posicion(posicionCarta){

    QPixmap pix(rutaImg);
    setPixmap(pix.scaled(130, 190, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setAcceptHoverEvents(true);
    setCursor(Qt::PointingHandCursor);
}

void CartaUi::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit cartaSeleccionada(this->posicion);
}

void CartaUi::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    setY(y() - 25);
}

void CartaUi::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    setY(y() + 25);
}
