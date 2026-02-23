#include "cartamanowidget.h"
#include <QPixmap>
#include <QCursor>
CartaManoWidget::CartaManoWidget(int idCarta, QString rutaImg, QGraphicsItem* parent):
    QObject(), QGraphicsPixmapItem(parent), id(idCarta){

    QPixmap pix(rutaImg);
    setPixmap(pix.scaled(130, 190, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setAcceptHoverEvents(true);
    setCursor(Qt::PointingHandCursor);
}
void CartaManoWidget::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit cartaSeleccionada(this->id);
}

void CartaManoWidget::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    setY(y() - 25);
}

void CartaManoWidget::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    setY(y() + 25);
}

