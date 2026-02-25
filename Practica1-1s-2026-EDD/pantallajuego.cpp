#include "pantallajuego.h"
#include "botonanimado.h"
#include "ui_pantallajuego.h"
#include "cartamanowidget.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QTimer>

PantallaJuego::PantallaJuego(int cantidadJugadores, bool esFlip, bool acumulacion, bool retoMasCuatro, bool robarSinLimite, bool gritoUno, bool ganarConNegra, QWidget *parent)
    : QWidget(parent), ui(new Ui::PantallaJuego)
      //* Inicializar el puntero de juego
      ,
      juego(new Juego(this)), numJugadores(cantidadJugadores), modoJuego(esFlip),
      retoActivo(retoMasCuatro), gritoUnoActivo(gritoUno)
{
    juego->setAcumulacion(acumulacion);
    juego->setRetoMasCuatro(retoMasCuatro);
    juego->setRobarSinLimite(robarSinLimite);
    juego->setGritoUno(gritoUno);
    juego->setGanarConNegra(ganarConNegra);
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

    ui->btnRetar->setVisible(retoActivo);
    ui->btnRetar->setEnabled(false);
    ui->btnAvisarUNO->setVisible(gritoUnoActivo);
    ui->btnReportar->setVisible(gritoUnoActivo);

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

    connect(juego, &Juego::debeJugarAntesDeRobarSignal,
            this, &PantallaJuego::onDebeJugarAntesDeRobar);

    connect(juego, &Juego::mazoSinCartasSignal,
            this, &PantallaJuego::onMazoSinCartas);

    connect(juego, &Juego::pedirColorSignal,
            this, &PantallaJuego::onPedirColor);

    connect(juego, &Juego::pedirDatosAdivinarSignal,
            this, &PantallaJuego::onPedirDatosAdivinar);

    connect(juego, &Juego::retoPosibleSignal,
            this, &PantallaJuego::onRetoPosible);

    connect(juego, &Juego::cartaNegraBloqueadaSignal,
            this, &PantallaJuego::onCartaNegraBloqueada);

    connect(juego, &Juego::retoResultadoSignal, this, [this](bool exito)
            {
        if (exito)
            QMessageBox::information(this, "¡Reto exitoso!",
                "El lanzador tenía una carta válida. ¡Roba 4 cartas!");
        else
            QMessageBox::warning(this, "Reto fallido",
                "El lanzador no tenía otra opción. Robas 6 cartas."); });

    connect(juego, &Juego::unoReportadoSignal, this, [this](bool fueValido, QString nombre)
            {
        if (fueValido)
            QMessageBox::warning(this, "¡UNO no dicho!",
                QString("%1 no dijo UNO. ¡Roba 2 cartas!").arg(nombre));
        else
            QMessageBox::information(this, "Reporte incorrecto",
                QString("%1 reportó sin razón. ¡Roba 2 cartas!").arg(nombre)); });

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
    if (actualizandoMano)
        return;

    if (retoActivo)
        ui->btnRetar->setEnabled(false);

    actualizandoMano = true;

    try
    {
        // DESCONECTAR TODAS las señales PRIMERO
        const QList<QGraphicsItem *> items = escena->items();
        for (QGraphicsItem *item : items)
        {
            CartaManoWidget *carta = dynamic_cast<CartaManoWidget *>(item);
            if (carta)
            {
                disconnect(carta, nullptr, this, nullptr);
            }
        }

        // Limpiar escena
        escena->clear();

        Jugador *jugadorActual = juego->getJugadorActual();
        if (!jugadorActual)
        {
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
            QTimer::singleShot(0, this, [this]() { emit salirPartida(); });
            return;
        }

        // --- Posicionamiento centrado usando el ancho real del widget visible ---
        const double ANCHO_CARTA = 90.0;
        const double ALTO_CARTA = 135.0;
        const double MARGEN = 10.0;

        double anchoContenedor = ui->manoJugadorWidget->width();
        if (anchoContenedor <= 0)
            anchoContenedor = 800.0;

        // Separación dinámica: se ajusta para que todas las cartas quepan centradas
        double espacioDisponible = anchoContenedor - ANCHO_CARTA - (MARGEN * 2.0);
        double separacion = (total > 1)
                                ? qBound(15.0, espacioDisponible / (total - 1), ANCHO_CARTA + 5.0)
                                : 0.0;

        double anchoTotal = ANCHO_CARTA + (total - 1) * separacion;
        double xInicio = qMax(MARGEN, (anchoContenedor - anchoTotal) / 2.0);
        double yInicio = (200.0 - ALTO_CARTA) / 2.0;

        escena->setSceneRect(0, 0, anchoContenedor, 200.0);
        vista->setSceneRect(0, 0, anchoContenedor, 200.0);

        // CREAR CARTAS — ID del widget == índice en la mano del jugador
        for (int i = 0; i < total; i++)
        {
            Carta carta = mano.obtenerElementoEnPosicion(i);
            if (!carta.esValida())
                continue;

            LadoCarta *ladoActivo = carta.getLadoActivo();
            if (!ladoActivo)
                continue;

            QString rutaImagen = QString::fromStdString(ladoActivo->getRutaArchivo());

            CartaManoWidget *cartaWidget = new CartaManoWidget(i, rutaImagen, nullptr);
            cartaWidget->setSize(ANCHO_CARTA, ALTO_CARTA);
            cartaWidget->setPos(xInicio + i * separacion, yInicio);
            cartaWidget->setZValue(i);

            escena->addItem(cartaWidget);
        }

        // CONECTAR señales de cada carta inmediatamente
        {
            const QList<QGraphicsItem *> itemsConexion = escena->items();
            int cartasConectadas = 0;

            for (QGraphicsItem *item : itemsConexion)
            {
                CartaManoWidget *carta = dynamic_cast<CartaManoWidget *>(item);
                if (carta)
                {
                    connect(carta, &CartaManoWidget::cartaSeleccionada,
                            this, &PantallaJuego::onCartaSeleccionada, Qt::UniqueConnection);
                    cartasConectadas++;
                }
            }
        }

        ui->lblCantidadCartas->setText(QString("Cartas: %1").arg(total));
    }
    catch (...)
    {
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
    QPushButton *btn = ui->pilaCartas->findChild<QPushButton *>("btnDescarte");
    if (!btn)
        return;

    QPixmap pixmap;
    // Intentar cargar la imagen; si falla usar el dorso genérico
    if (!pixmap.load(rutaCarta))
    {
        pixmap.load(":/assets/VARIOS/POSTERIOR.png");
    }

    if (!pixmap.isNull())
    {
        btn->setIcon(QIcon(pixmap.scaled(100, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
        btn->setIconSize(QSize(100, 150));
    }
}

void PantallaJuego::onCartaSeleccionada(int idCartaOriginal)
{
    // PantallaJuego solo delega — toda lógica y validación es responsabilidad de Juego
    static bool bloqueado = false;
    if (bloqueado || actualizandoMano)
        return;

    bloqueado = true;

    // Desconectar cartas para evitar re-disparo mientras Juego procesa
    const QList<QGraphicsItem *> items = escena->items();
    for (QGraphicsItem *item : items)
    {
        CartaManoWidget *carta = dynamic_cast<CartaManoWidget *>(item);
        if (carta)
            disconnect(carta, nullptr, this, nullptr);
    }

    // El ID del widget == índice en la mano (asignado en actualizarManoJugador)
    juego->onCartaJugadaSlot(idCartaOriginal);

    bloqueado = false;
}

void PantallaJuego::onRobarCarta()
{
    // PantallaJuego solo delega — Juego decide si se puede robar y actualiza el estado
    juego->intentarRobarCarta();
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

void PantallaJuego::onCartaInvalida()
{
    QMessageBox::warning(this, "Carta inválida",
                         "Esta carta no puede jugarse ahora.\n"
                         "Debe coincidir con el color o número de la carta en descarte.\n"
                         "Si no tienes carta válida, roba del mazo.");
}

void PantallaJuego::onDebeJugarAntesDeRobar()
{
    QMessageBox::warning(this, "No puedes robar",
                         "Tienes cartas que puedes jugar.\n"
                         "¡Debes jugar una carta primero!");
}

void PantallaJuego::onMazoSinCartas()
{
    QMessageBox::information(this, "Sin cartas",
                             "No hay cartas disponibles en el mazo ni en el descarte.");
}

void PantallaJuego::onPedirColor(int indiceCarta, bool modoOscuro)
{
    // Colores disponibles según el lado activo del juego
    QStringList colores;
    if (modoOscuro)
    {
        colores << "Naranja" << "Rosa" << "Turquesa" << "Purpura";
    }
    else
    {
        colores << "Rojo" << "Azul" << "Verde" << "Amarillo";
    }

    bool ok;
    QString colorElegido = QInputDialog::getItem(
        this,
        "Cambio de color",
        "Selecciona el color que quieres activar:",
        colores, 0, false, &ok);

    if (ok && !colorElegido.isEmpty())
    {
        juego->jugarCartaConColor(indiceCarta, colorElegido.toStdString());
    }
    else
    {
        // Jugador canceló: reconectar las señales para que pueda seguir jugando
        actualizarManoJugador();
    }
}

void PantallaJuego::onPedirDatosAdivinar(int indiceCarta)
{
    // Paso 1: Seleccionar la víctima (cualquier jugador excepto el actual)
    Jugador *actual = juego->getJugadorActual();
    QStringList nombresJugadores;
    for (int i = 0; i < juego->getCantidadJugadores(); i++)
    {
        Jugador *j = juego->getJugadorEnPosicion(i);
        if (j && j != actual)
        {
            nombresJugadores << QString::fromStdString(j->getNombreJugador());
        }
    }

    if (nombresJugadores.isEmpty())
    {
        actualizarManoJugador();
        return;
    }

    bool ok;
    QString victima = QInputDialog::getItem(
        this, "Adivinar Carta",
        "¿A qué jugador quieres adivinar?",
        nombresJugadores, 0, false, &ok);
    if (!ok || victima.isEmpty())
    {
        actualizarManoJugador();
        return;
    }

    // Paso 2: Seleccionar el color de la carta a adivinar
    QStringList colores;
    if (juego->getLadoOscuroActivo())
    {
        colores << "Naranja" << "Rosa" << "Turquesa" << "Purpura";
    }
    else
    {
        colores << "Rojo" << "Azul" << "Verde" << "Amarillo";
    }

    QString color = QInputDialog::getItem(
        this, "Adivinar Carta",
        "¿Qué color tiene la carta que estás adivinando?",
        colores, 0, false, &ok);
    if (!ok || color.isEmpty())
    {
        actualizarManoJugador();
        return;
    }

    // Paso 3: Ingresar el número de la carta
    int numero = QInputDialog::getInt(
        this, "Adivinar Carta",
        "¿Qué número tiene la carta? (0-9):",
        0, 0, 9, 1, &ok);
    if (!ok)
    {
        actualizarManoJugador();
        return;
    }

    juego->jugarCartaAdivinar(indiceCarta, color.toStdString(), victima.toStdString(), numero);
}

void PantallaJuego::onRetoPosible()
{
    ui->btnRetar->setEnabled(true);
}

void PantallaJuego::on_btnRetar_clicked()
{
    if (!juego->getRetoPendiente())
        return;
    juego->resolverReto();
}

void PantallaJuego::onCartaNegraBloqueada()
{
    QMessageBox::warning(this, "Carta bloqueada",
                         "No puedes ganar con una carta negra (comodín)."

                         "Debes robar una carta hasta poder jugar una de color.");
}

void PantallaJuego::on_btnAvisarUNO_clicked()
{
    juego->avisarUno();
}

void PantallaJuego::on_btnReportar_clicked()
{
    juego->reportarUno();
}
