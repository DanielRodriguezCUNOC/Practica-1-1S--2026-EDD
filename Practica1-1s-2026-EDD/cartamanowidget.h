#ifndef CARTAMANOWIDGET_H
#define CARTAMANOWIDGET_H

#include <QObject>
#include <QGraphicsPixmapItem>

class CartaManoWidget: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

private:
    int id;
public:
    CartaManoWidget(int id, QString rutaImagen, QGraphicsItem* parent=nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
signals:
    void cartaSeleccionada(int id);
};

#endif // CARTAMANOWIDGET_H
