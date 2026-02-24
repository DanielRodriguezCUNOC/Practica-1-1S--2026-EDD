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

    connect(juego, &Juego::cartaInvalidaSignal,
            this, &PantallaJuego::onCartaInvalida);

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
    qDebug() << "[PANTALLA] === actualizarManoJugador INICIADO ===";

    if (actualizandoMano) {
        qDebug() << "[PANTALLA] Ya actualizando mano, saliendo";
        return;
    }

    actualizandoMano = true;

    try {
        // DESCONECTAR TODAS las señales PRIMERO
        const QList<QGraphicsItem *> items = escena->items();
        for (QGraphicsItem *item : items) {
            CartaManoWidget *carta = dynamic_cast<CartaManoWidget *>(item);
            if (carta) {
                disconnect(carta, nullptr, this, nullptr);
            }
        }

        // Limpiar escena
        escena->clear();

        Jugador *jugadorActual = juego->getJugadorActual();
        if (!jugadorActual) {
            qDebug() << "[PANTALLA ERROR] No hay jugador actual";
            actualizandoMano = false;
            return;
        }

        ListaGenerica<Carta> &mano = jugadorActual->getMano();
        int total = mano.getSize();

        qDebug() << "[PANTALLA] Jugador actual tiene" << total << "cartas";

        if (total == 0) {
            actualizandoMano = false;
            QMessageBox::information(this, "¡Ganador!",
                                     QString("%1 ha ganado la partida!").arg(QString::fromStdString(jugadorActual->getNombreJugador())));
            return;
        }

        // Calcular posicionamiento
        double anchoContenedor = this->vista->viewport()->width();
        if (anchoContenedor < 1241) anchoContenedor = 1241;

        double anchoCarta = 135.0;
        double ideal = (total > 1) ? (anchoContenedor - 30 - anchoCarta) / (total - 1) : 0;
        double separacion = (total > 1) ? qBound(25.0, ideal, 85.0) : 0;
        double anchoTotalCartas = anchoCarta + (total - 1) * separacion;
        double xActual = (anchoContenedor - anchoTotalCartas) / 2.0;

        this->escena->setSceneRect(0, 0, anchoContenedor, 240);

        // CREAR CARTAS CON ÍNDICES CORRECTOS
        for (int i = 0; i < total; i++) {
            Carta carta = mano.obtenerElementoEnPosicion(i);
            if (!carta.esValida()) {
                qDebug() << "[PANTALLA ERROR] Carta en posición" << i << "no es válida";
                continue;
            }

            LadoCarta *ladoActivo = carta.getLadoActivo();
            if (!ladoActivo) {
                qDebug() << "[PANTALLA ERROR] Lado activo nulo en posición" << i;
                continue;
            }

            QString rutaImagen = QString::fromStdString(ladoActivo->getRutaArchivo());

            // CREAR CARTA CON ÍNDICE CORRECTO
            CartaManoWidget *cartaWidget = new CartaManoWidget(i, rutaImagen, nullptr);
            cartaWidget->setPos(xActual, 0);

            qDebug() << "[PANTALLA] Creada carta" << i << "en posición" << xActual;

            // Agregar a la escena
            escena->addItem(cartaWidget);

            xActual += separacion;
        }

        // CONECTAR señales de cada carta inmediatamente
        {
            const QList<QGraphicsItem *> itemsConexion = escena->items();
            int cartasConectadas = 0;

            for (QGraphicsItem *item : itemsConexion) {
                CartaManoWidget *carta = dynamic_cast<CartaManoWidget *>(item);
                if (carta) {
                    connect(carta, &CartaManoWidget::cartaSeleccionada,
                            this, &PantallaJuego::onCartaSeleccionada, Qt::UniqueConnection);
                    cartasConectadas++;
                }
            }
            qDebug() << "[PANTALLA] Conectadas" << cartasConectadas << "cartas";
        }

        ui->lblCantidadCartas->setText(QString("Cartas: %1").arg(total));

        qDebug() << "[PANTALLA] Mano actualizada correctamente con" << total << "cartas";

    } catch (...) {
        qDebug() << "[PANTALLA ERROR CRÍTICO] Excepción en actualizarManoJugador";
    }

    actualizandoMano = false;
    qDebug() << "[PANTALLA] === actualizarManoJugador FINALIZADO ===";

    qDebug() << "[PANTALLA DEBUG] === CARTAS CREADAS ===";
    const QList<QGraphicsItem *> itemsDebug = escena->items();
    for (QGraphicsItem *item : itemsDebug) {
        CartaManoWidget *carta = dynamic_cast<CartaManoWidget *>(item);
        if (carta) {
            qDebug() << "[PANTALLA] Carta ID:" << carta->getIdCarta() << "Pos X:" << carta->pos().x();
        }
    }
    qDebug() << "[PANTALLA DEBUG] === FIN CARTAS ===";
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


void PantallaJuego::onCartaSeleccionada(int idCartaOriginal)
{
    qDebug() << "[PANTALLA] === onCartaSeleccionada INICIADO ===";
    qDebug() << "[PANTALLA] ID Carta seleccionada:" << idCartaOriginal;

    static bool bloqueado = false;
    if (bloqueado || actualizandoMano) {
        qDebug() << "[PANTALLA] BLOQUEADO";
        return;
    }

    bloqueado = true;

    // DESCONECTAR TODAS las cartas inmediatamente para evitar re-disparo
    {
        const QList<QGraphicsItem *> itemsViejos = escena->items();
        for (QGraphicsItem *item : itemsViejos) {
            CartaManoWidget *carta = dynamic_cast<CartaManoWidget *>(item);
            if (carta) {
                disconnect(carta, nullptr, this, nullptr);
            }
        }
    }

    try {
        if (!juego) {
            qDebug() << "[PANTALLA ERROR] Juego es nullptr";
            bloqueado = false;
            return;
        }

        Jugador* jugadorActual = juego->getJugadorActual();
        if (!jugadorActual) {
            qDebug() << "[PANTALLA ERROR] No hay jugador actual";
            bloqueado = false;
            return;
        }

        // Obtener todas las cartas de la escena
        const QList<QGraphicsItem *> items = escena->items();

        // Arrays manuales para ordenamiento
        CartaManoWidget* cartasArray[20];  // Máximo 20 cartas
        double posicionesX[20];
        int totalCartas = 0;

        // Recopilar cartas y sus posiciones
        for (QGraphicsItem *item : items) {
            CartaManoWidget *carta = dynamic_cast<CartaManoWidget *>(item);
            if (carta && totalCartas < 20) {
                cartasArray[totalCartas] = carta;
                posicionesX[totalCartas] = carta->pos().x();
                totalCartas++;
            }
        }

        qDebug() << "[PANTALLA] Encontradas" << totalCartas << "cartas en escena";

        // ORDENAMIENTO BURBUJA MANUAL por posición X
        for (int i = 0; i < totalCartas - 1; i++) {
            for (int j = 0; j < totalCartas - 1 - i; j++) {
                if (posicionesX[j] > posicionesX[j + 1]) {
                    // Intercambiar posiciones
                    double tempPos = posicionesX[j];
                    posicionesX[j] = posicionesX[j + 1];
                    posicionesX[j + 1] = tempPos;

                    // Intercambiar cartas
                    CartaManoWidget* tempCarta = cartasArray[j];
                    cartasArray[j] = cartasArray[j + 1];
                    cartasArray[j + 1] = tempCarta;
                }
            }
        }

        // BUSCAR la posición de la carta seleccionada EN EL ARRAY ORDENADO
        int posicionVisual = -1;
        for (int i = 0; i < totalCartas; i++) {
            if (cartasArray[i]->getIdCarta() == idCartaOriginal) {
                posicionVisual = i;  // Esta ES la posición real en la mano
                break;
            }
        }

        if (posicionVisual == -1) {
            qDebug() << "[PANTALLA ERROR] Carta no encontrada en array ordenado";
            bloqueado = false;
            return;
        }

        qDebug() << "[PANTALLA] Carta ID" << idCartaOriginal << "está en posición visual" << posicionVisual;

        // VALIDAR que la posición existe en la mano actual
        int cartasEnMano = jugadorActual->getMano().getSize();
        if (posicionVisual >= cartasEnMano) {
            qDebug() << "[PANTALLA ERROR] Posición" << posicionVisual << "fuera de rango de" << cartasEnMano;
            bloqueado = false;
            return;
        }

        // VALIDAR que la carta existe
        Carta cartaSeleccionada = jugadorActual->getMano().obtenerElementoEnPosicion(posicionVisual);
        if (!cartaSeleccionada.esValida()) {
            qDebug() << "[PANTALLA ERROR] Carta en posición" << posicionVisual << "no es válida";
            bloqueado = false;
            return;
        }

        qDebug() << "[PANTALLA] ✅ Procesando carta en posición REAL" << posicionVisual;

        // PROCESAR con la posición CORRECTA
        juego->onCartaJugadaSlot(posicionVisual);

    } catch (...) {
        qDebug() << "[PANTALLA ERROR CRÍTICO] Excepción en onCartaSeleccionada";
    }

    bloqueado = false;
    qDebug() << "[PANTALLA] === onCartaSeleccionada FINALIZADO ===";
}

void PantallaJuego::onRobarCarta()
{
    qDebug() << "[PANTALLA] === onRobarCarta INICIADO ===";

    Jugador* jugadorActual = juego->getJugadorActual();
    if (!jugadorActual) return;

    // ✅ Verificar si el jugador tiene alguna carta válida para jugar
    bool tieneCartaValida = false;
    ListaGenerica<Carta>& mano = jugadorActual->getMano();

    for (int i = 0; i < mano.getSize(); i++) {
        Carta carta = mano.obtenerElementoEnPosicion(i);
        if (juego->puedeJugarCarta(carta)) {
            tieneCartaValida = true;
            break;
        }
    }

    if (tieneCartaValida) {
        qDebug() << "[PANTALLA] Jugador tiene cartas válidas, no puede robar";
        QMessageBox::warning(this, "No puedes robar",
                             "Tienes cartas que puedes jugar.\n¡Debes jugar una carta primero!");
        return;
    }

    // ✅ Robar carta del mazo
    if (juego->mazoEstaVacio()) {
        qDebug() << "[PANTALLA] Mazo vacío, barajando descarte...";
        juego->barajarDescarte();
    }

    Carta nuevaCarta = juego->robarDelMazo();
    if (nuevaCarta.esValida()) {
        jugadorActual->agregarCarta(nuevaCarta);
        qDebug() << "[PANTALLA] Carta robada agregada a la mano";

        // ✅ Actualizar la mano visualmente
        actualizarManoJugador();

        // ✅ Pasar turno automáticamente después de robar
        juego->avanzarTurno();

        Jugador* nuevoJugador = juego->getJugadorActual();
        if (nuevoJugador) {
            actualizarTurno(QString::fromStdString(nuevoJugador->getNombreJugador()));
        }

        qDebug() << "[PANTALLA] Turno avanzado tras robar carta";
    } else {
        QMessageBox::information(this, "Mazo vacío", "No hay cartas disponibles para robar.");
    }

    qDebug() << "[PANTALLA] === onRobarCarta FINALIZADO ===";
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

// ...existing code...
void PantallaJuego::onCartaInvalida()
{
    qDebug() << "[PANTALLA] Carta inválida - no coincide color/número";
    QMessageBox::warning(this, "Carta inválida",
                         "Esta carta no puede jugarse ahora.\n"
                         "Debe coincidir con el color o número de la carta en descarte.\n"
                         "Si no tienes carta válida, roba del mazo.");
}
