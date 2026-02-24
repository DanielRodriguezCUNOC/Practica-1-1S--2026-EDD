#include "cartamanowidget.h"
#include <QPixmap>
#include <QCursor>
#include <QFileInfo>
CartaManoWidget::CartaManoWidget(int idCarta, QString rutaImg, QGraphicsItem* parent):
    QObject(), QGraphicsPixmapItem(parent), id(idCarta){

    QString rutaValida = buscarImagenAlternativa(rutaImg);

    QPixmap pixmap;
    if (!pixmap.load(rutaValida)) {
        qDebug() << "ERROR CRÍTICO: No se pudo cargar ninguna imagen para:" << rutaImg;
        // Crear un rectángulo de color como último recurso
        pixmap = QPixmap(135, 180);
        pixmap.fill(Qt::darkGray);
    }

    setPixmap(pixmap);
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
}
void CartaManoWidget::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit cartaSeleccionada(this->id);
    QGraphicsPixmapItem:mousePressEvent(event);
}

void CartaManoWidget::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    setY(y() - 25);
}

void CartaManoWidget::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    setY(y() + 25);
}

void CartaManoWidget::setSize(double ancho, double alto){
    setScale(ancho/boundingRect().width());
}

bool CartaManoWidget::imagenValida()const{
    return !pixmap().isNull();
}



QString CartaManoWidget::buscarImagenAlternativa(QString rutaOriginal) {
    // Si la imagen original existe, usarla
    if (QFile::exists(rutaOriginal)) {
        return rutaOriginal;
    }

    qDebug() << "Imagen no encontrada, buscando alternativa para:" << rutaOriginal;

    // Estrategia 1: Para cartas de comodín (están en carpeta COMODINES)
    if (rutaOriginal.contains("COMODINES") ||
        rutaOriginal.contains("Comodin") ||
        rutaOriginal.contains("+4") ||
        rutaOriginal.contains("+6")) {

        // Extraer el nombre del archivo
        QFileInfo info(rutaOriginal);
        QString nombreArchivo = info.fileName();

        // Buscar en la otra carpeta de comodines
        if (rutaOriginal.contains("/NORMAL/COMODINES/")) {
            QString alternativa = ":/assets/FLIP/COMODINES/" + nombreArchivo;
            if (QFile::exists(alternativa)) {
                qDebug() << "  Usando comodín FLIP:" << alternativa;
                return alternativa;
            }
        }
        else if (rutaOriginal.contains("/FLIP/COMODINES/")) {
            QString alternativa = ":/assets/NORMAL/COMODINES/" + nombreArchivo;
            if (QFile::exists(alternativa)) {
                qDebug() << "  Usando comodín NORMAL:" << alternativa;
                return alternativa;
            }
        }
    }

    // Estrategia 2: Para cartas de color (están en carpetas de colores)
    // Si es NORMAL con color oscuro, buscar en FLIP
    if (rutaOriginal.contains("/NORMAL/")) {
        if (rutaOriginal.contains("ROSADO") ||
            rutaOriginal.contains("TURQUESA") ||
            rutaOriginal.contains("NARANJA") ||
            rutaOriginal.contains("PURPURA")) {

            QString alternativa = rutaOriginal;
            alternativa.replace("/NORMAL/", "/FLIP/");
            if (QFile::exists(alternativa)) {
                qDebug() << "  Usando alternativa FLIP:" << alternativa;
                return alternativa;
            }
        }
    }

    // Estrategia 3: Si es FLIP con color claro, buscar en NORMAL
    if (rutaOriginal.contains("/FLIP/")) {
        if (rutaOriginal.contains("ROJO") ||
            rutaOriginal.contains("AZUL") ||
            rutaOriginal.contains("VERDE") ||
            rutaOriginal.contains("AMARILLO")) {

            QString alternativa = rutaOriginal;
            alternativa.replace("/FLIP/", "/NORMAL/");
            if (QFile::exists(alternativa)) {
                qDebug() << "  Usando alternativa NORMAL:" << alternativa;
                return alternativa;
            }
        }
    }

    // Estrategia 4: Intentar con el dorso genérico
    QString dorso = ":/assets/VARIOS/POSTERIOR.png";
    if (QFile::exists(dorso)) {
        qDebug() << "  Usando dorso genérico:" << dorso;
        return dorso;
    }

    // Último recurso: devolver la original (fallará, pero ya manejamos eso)
    qDebug() << "  No se encontró ninguna alternativa";
    return rutaOriginal;
}
