#ifndef CARTAMANOWIDGET_H
#define CARTAMANOWIDGET_H

#include <QObject>
#include <QGraphicsPixmapItem>

class CartaManoWidget: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

private:
    int id;
    QString buscarImagenAlternativa(QString rutaOriginal);
public:
    CartaManoWidget(int id, QString rutaImagen, QGraphicsItem* parent=nullptr);
    void setSize(double ancho, double alto);
    bool imagenValida()const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
signals:
    void cartaSeleccionada(int id);
};

#endif // CARTAMANOWIDGET_H
