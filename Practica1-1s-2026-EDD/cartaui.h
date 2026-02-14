#ifndef CARTAUI_H
#define CARTAUI_H

#include <QObject>
#include <QGraphicsPixmapItem>


class CartaUi: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    //* Constructor
    CartaUi(int posicionCarta, QString rutaImg, QGraphicsItem* parent=nullptr );

signals:
    //* Pasamos el id de la carta seleccionada
    void cartaSeleccionada(int id);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
private:
    int posicion;
};

#endif // CARTAUI_H
