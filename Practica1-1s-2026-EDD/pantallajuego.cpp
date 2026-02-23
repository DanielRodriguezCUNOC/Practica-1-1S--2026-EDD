#include "pantallajuego.h"
#include "botonanimado.h"
#include "ui_pantallajuego.h"
#include "cartamanowidget.h"

PantallaJuego::PantallaJuego(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaJuego)
    //* Inicializar el puntero de juego
    , juego(new Juego())
{
    ui->setupUi(this);
    this ->setObjectName("PantallaJuego");
    connect(ui->btnSalirJuego, &BotonAnimado::clicked, this, &PantallaJuego::salirPartida);

    this->setAttribute(Qt::WA_StyledBackground, true);
    //* Colocar imagen de fondo
    this->setStyleSheet("QWidget#PantallaJuego {"
                        "border-image: url(:/assets/VARIOS/Board.png) 0 0 0 0 stretch stretch;"
                        "background-repeat: no-repeat;"
                        "background-position: center;"
                        "margin: 0px;"
                        "padding: 0px;"
                        "}");

    escena = new QGraphicsScene(this);
    vista = new QGraphicsView(escena, this);
    vista->setStyleSheet("background: transparent; border: none;");
    vista->setRenderHint(QPainter::Antialiasing);

    //Usamos el widget para mostrar la mano
    if(ui->manoJugadorWidget->layout()){
        ui->manoJugadorWidget->layout()->addWidget(vista);
    }else{
        QVBoxLayout* layoutInterno = new QVBoxLayout(ui->manoJugadorWidget);
        layoutInterno->addWidget(vista);
    }

    this->escena->clear();

    int total = juego->getJugadorActual()->getMano().getSize();
    if (total == 0) return;

    double anchoContenedor = this->vista->viewport()->width();
    if (anchoContenedor < 1241) anchoContenedor = 1241;

    double anchoCarta = 135.0;
    double ideal = (total > 1) ? (anchoContenedor - 30 - anchoCarta) / (total - 1) : 0;

    double separacion = (total > 1) ? qBound(25.0, ideal, 85.0) : 0;

    double anchoTotalCartas = anchoCarta + (total - 1) * separacion;
    double xActual = (anchoContenedor - anchoTotalCartas) / 2.0;

    this->escena->setSceneRect(0, 0, anchoContenedor, 240);

}

PantallaJuego::~PantallaJuego()
{
    delete juego;
    delete ui;
}
void PantallaJuego:: iniciarNuevaPartida(int numJugadores, bool esFlip){
    //* Inicializar el mazo segun el modo
    juego->inicializarMazo(esFlip, numJugadores);


}

void PantallaJuego::dibujarMazo(ListaGenerica<Carta> mazo){
    escena->clear(); // Limpia cartas anteriores

    int total = mazo.getSize();
    if (total == 0) return;

    // --- CONFIGURACIÓN DE LAYOUT DINÁMICO ---
    double anchoContenedor = 900; // El ancho que quieras usar de la pantalla
    double xActual = 50;          // Margen inicial

    // Si hay muchas cartas, se solapan (20px min). Si hay pocas, se separan (140px max)
    double separacion = qMin(140.0, anchoContenedor / total);

    for (int i = 0; i < total; ++i) {
        // Suponiendo que mazoBackend[i] tiene el ID y la Ruta de la imagen
        CartaManoWidget* cartaAMostrar = new CartaManoWidget(i, QString::fromStdString(mazo.obtenerElementoEnPosicion(i).getLadoActivo()->getRutaArchivo()));

        // Conectas la carta al slot de esta pantalla
        connect(cartaAMostrar, &CartaManoWidget::cartaSeleccionada, this, &PantallaJuego::onCartaSeleccionada);

        escena->addItem(cartaAMostrar);
        cartaAMostrar->setPos(xActual + (i * separacion), 300); // 300 es la altura en el tapete
        cartaAMostrar->setZValue(i); // Para que se solapen correctamente de izq a der
}
}

void PantallaJuego::on_btnSalirJuego_clicked() {}
