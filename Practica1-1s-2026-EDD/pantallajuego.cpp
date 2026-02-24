#include "pantallajuego.h"
#include "botonanimado.h"
#include "ui_pantallajuego.h"
#include "cartamanowidget.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QTimer>

PantallaJuego::PantallaJuego(int cantidadJugadores, bool esFlip, QWidget *parent)
    : QWidget(parent), ui(new Ui::PantallaJuego)
      //* Inicializar el puntero de juego
      ,
      juego(new Juego(this)), numJugadores(cantidadJugadores), modoJuego(esFlip)
{
    ui->setupUi(this);

    ui->manoJugadorWidget->resize(1241, 240);

    this->setObjectName("PantallaJuego");

    configurarConexiones();

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

    ui->btnMazo->setIcon(QIcon(":/assets/VARIOS/POSTERIOR.png"));
    ui->btnMazo->setIconSize(QSize(120, 180));

    escena = new QGraphicsScene(this);
    vista = new QGraphicsView(escena, ui->manoJugadorWidget);
    vista->setStyleSheet("background: transparent; border: none;");
    vista->setRenderHint(QPainter::Antialiasing);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Layout para manoJugadorWidget
    QVBoxLayout *layout = new QVBoxLayout(ui->manoJugadorWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(vista);
    ui->manoJugadorWidget->setLayout(layout);

    this->escena->clear();

    iniciarNuevaPartida(numJugadores, modoJuego);

    /*int total = juego->getJugadorActual()->getMano().getSize();
    if (total == 0) return;
    double anchoContenedor = this->vista->viewport()->width();
    if (anchoContenedor < 1241) anchoContenedor = 1241;

    double anchoCarta = 135.0;
    double ideal = (total > 1) ? (anchoContenedor - 30 - anchoCarta) / (total - 1) : 0;

    double separacion = (total > 1) ? qBound(25.0, ideal, 85.0) : 0;

    double anchoTotalCartas = anchoCarta + (total - 1) * separacion;
    double xActual = (anchoContenedor - anchoTotalCartas) / 2.0;

    this->escena->setSceneRect(0, 0, anchoContenedor, 240);
    */
}

PantallaJuego::~PantallaJuego()
{
    delete juego;
    delete ui;
}
void PantallaJuego::iniciarNuevaPartida(int numJugadores, bool esFlip)
{

    int jugadoresAgregados = 0;
    bool ok;

    do
    {

        // Mostramos el diálogo para pedir el texto
        QString nombreIngresado = QInputDialog::getText(this,
                                                        "Nuevo Jugador",
                                                        "Ingresa el nombre del jugador:",
                                                        QLineEdit::Normal,
                                                        "",
                                                        &ok);

        // Verificamos si presionó Aceptar y si no dejó el espacio en blanco
        if (ok && !nombreIngresado.isEmpty())
        {

            std::string nombreStdString = nombreIngresado.toStdString();

            juego->setJugadorEnLista(nombreStdString);

            QMessageBox::information(this, "Éxito", "Jugador agregado: " + nombreIngresado);
            jugadoresAgregados++;
        }
        else if (ok && nombreIngresado.isEmpty())
        {
            QMessageBox::warning(this, "Cuidado", "El nombre no puede estar vacío.");
        }

    } while (jugadoresAgregados < numJugadores);

    //* Inicializar el mazo segun el modo
    juego->inicializarMazo(numJugadores, esFlip);
}

void PantallaJuego::dibujarMazo(ListaGenerica<Carta> mazo)
{
    escena->clear(); // Limpia cartas anteriores

    int total = mazo.getSize();
    if (total == 0)
        return;

    // --- CONFIGURACIÓN DE LAYOUT DINÁMICO ---
    double anchoContenedor = 900; // El ancho que quieras usar de la pantalla
    double xActual = 50;          // Margen inicial

    // Si hay muchas cartas, se solapan (20px min). Si hay pocas, se separan (140px max)
    double separacion = qMin(140.0, anchoContenedor / total);

    for (int i = 0; i < total; ++i)
    {
        // Suponiendo que mazoBackend[i] tiene el ID y la Ruta de la imagen
        CartaManoWidget *cartaAMostrar = new CartaManoWidget(i, QString::fromStdString(mazo.obtenerElementoEnPosicion(i).getLadoActivo()->getRutaArchivo()));

        // Conectas la carta al slot de esta pantalla
        connect(cartaAMostrar, &CartaManoWidget::cartaSeleccionada, this, &PantallaJuego::onCartaSeleccionada);

        escena->addItem(cartaAMostrar);
        cartaAMostrar->setPos(xActual + (i * separacion), 300); // 300 es la altura en el tapete
        cartaAMostrar->setZValue(i);                            // Para que se solapen correctamente de izq a der
    }
}

void PantallaJuego::configurarConexiones()
{
    // Conectar señales del juego con slots de la pantalla
    connect(juego, &Juego::manoActualizadaSignal,
            this, &PantallaJuego::actualizarManoJugador);

    connect(juego, &Juego::turnoCambiadoSignal,
            this, &PantallaJuego::actualizarTurno);

    connect(juego, &Juego::descarteActualizadoSignal,
            this, &PantallaJuego::actualizarDescarte);

    connect(juego, &Juego::partidaIniciadaSignal,
            this, &PantallaJuego::onPartidaIniciada);

    // Conectar botones de acción
    connect(ui->btnRobarCarta, &BotonAnimado::clicked,
            this, &PantallaJuego::onRobarCarta);
    connect(ui->btnPasarTurno, &BotonAnimado::clicked,
            this, &PantallaJuego::onPasarTurno);
}

void PantallaJuego::onPartidaIniciada()
{

    actualizarManoJugador();

    actualizarTurno(QString::fromStdString(
        juego->getJugadorActual()->getNombreJugador()));
}

void PantallaJuego::actualizarManoJugador()
{

    if (actualizandoMano){
         qDebug() << "Ya se está actualizando la mano, ignorando llamada";
        return;
    }
    actualizandoMano = true;

    try {
        const QList<QGraphicsItem *> items = escena->items();
        for (QGraphicsItem *item : items)
        {
            CartaManoWidget *carta = dynamic_cast<CartaManoWidget *>(item);
            if (carta)
            {
                disconnect(carta, &CartaManoWidget::cartaSeleccionada,
                           this, &PantallaJuego::onCartaSeleccionada);
            }
        }

        // Limpiar escena actual
        escena->clear();

        Jugador *jugadorActual = juego->getJugadorActual();
        if (!jugadorActual) {
            actualizandoMano = false;
            return;
        }

        ListaGenerica<Carta> &mano = jugadorActual->getMano();
        int total = mano.getSize();
        if (total == 0)
        {
            actualizandoMano = false;
            QMessageBox::information(this, "¡Ganador!",
                                     QString("%1 ha ganado la partida!").arg(QString::fromStdString(jugadorActual->getNombreJugador())));
            return;
        }

    // Configurar la escena
    double anchoContenedor = ui->manoJugadorWidget->width() - 20;
    double altoContenedor = ui->manoJugadorWidget->height() - 20;

    double anchoCarta = 135.0;
    double separacionMinima = 25.0;
    double separacionMaxima = 85.0;

    double separacion;
    if (total > 1)
    {
        separacion = (anchoContenedor - anchoCarta) / (total - 1);
        separacion = qBound(separacionMinima, separacion, separacionMaxima);
    }
    else
    {
        separacion = 0;
    }

    double anchoTotalCartas = anchoCarta + (total - 1) * separacion;
    double xInicial = (anchoContenedor - anchoTotalCartas) / 2.0;

    if (xInicial < 10)
        xInicial = 10;

    escena->setSceneRect(0, 0, anchoContenedor + 20, altoContenedor);

    for (int i = 0; i < total; ++i)
    {
        Carta carta = mano.obtenerElementoEnPosicion(i);
        QString ruta = QString::fromStdString(carta.getLadoActivo()->getRutaArchivo());

        // Asegurar que la ruta tenga el prefijo correcto si es necesario
        if (!ruta.startsWith(":") && !ruta.startsWith("/"))
        {
            // Si es una ruta relativa, agregar el prefijo de recursos
            ruta = ":/assets/" + ruta; // Ajusta esto según tu estructura de carpetas
        }

        CartaManoWidget *cartaWidget = new CartaManoWidget(i, ruta);

        connect(cartaWidget, &CartaManoWidget::cartaSeleccionada,
                this, &PantallaJuego::onCartaSeleccionada);

        escena->addItem(cartaWidget);

        double yPos = (altoContenedor - 180) / 2.0; // Alto aproximado
        cartaWidget->setPos(xInicial + (i * separacion), yPos);
        cartaWidget->setZValue(i);
    }

    vista->setSceneRect(escena->itemsBoundingRect());
    vista->centerOn(escena->sceneRect().center());

    ui->lblCantidadCartas->setText(QString("Cartas: %1").arg(total));
    }catch(...){
        qDebug() << "Error en actualizarManoJugador";
    }
    actualizandoMano = false;
}

void PantallaJuego::actualizarTurno(QString nombreJugador)
{
    ui->lblTurnoActual->setText("Turno de: " + nombreJugador);

    // Actualizar estilo visual del turno
    QString estilo = QString("QLabel { color: white; font-size: 16px; "
                             "background-color: rgba(0,0,0,0.7); "
                             "padding: 5px; border-radius: 5px; }");
    ui->lblTurnoActual->setStyleSheet(estilo);
}

void PantallaJuego::actualizarDescarte(QString rutaCarta)
{

    // Buscar el botón dentro del widget contenedor
    QPushButton *btn = ui->pilaCartas->findChild<QPushButton *>("btnDescarte");

    if (!btn)
    {
        return;
    }

    QPixmap pixmap(rutaCarta);
    if (!pixmap.isNull())
    {
        QIcon icon(pixmap.scaled(100, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        btn->setIcon(icon);
        btn->setIconSize(QSize(100, 150));
    }
}

void PantallaJuego::onCartaSeleccionada(int idCarta)
{
    // Prevenir múltiples selecciones simultáneas
        if (actualizandoMano) {
        qDebug() << "Ignorando selección de carta durante actualización";
        return;
    }

    // Usar QTimer para evitar problemas de sincronización
    QTimer::singleShot(0, this, [this, idCarta]() {
        try {
            qDebug() << "Procesando carta seleccionada:" << idCarta;
            juego->onCartaJugadaSlot(idCarta);
        } catch (...) {
            qDebug() << "Error al procesar carta seleccionada:" << idCarta;
        }
    });
}

void PantallaJuego::onRobarCarta()
{
    Jugador *jugadorActual = juego->getJugadorActual();
    if (!jugadorActual)
        return;

    Carta nuevaCarta = juego->robarDelMazo();
    if (nuevaCarta.esValida())
    {
        jugadorActual->agregarCarta(nuevaCarta);
        juego->avanzarTurno();
    }
}

void PantallaJuego::onPasarTurno()
{
    juego->avanzarTurno();
}

void PantallaJuego::on_btnSalirJuego_clicked() {}

void PantallaJuego::on_btnRobarCarta_clicked()
{
    onRobarCarta();
}

void PantallaJuego::on_btnPasarTurno_clicked()
{
    onPasarTurno();
}
