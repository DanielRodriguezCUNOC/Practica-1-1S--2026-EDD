cmake_minimum_required(VERSION 3.19)
project(Practica1-1s-2026-EDD LANGUAGES CXX)

set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(Practica1-1s-2026-EDD
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    configuraciones.h configuraciones.cpp configuraciones.ui
    ayuda.h ayuda.cpp ayuda.ui
    pantallajuego.h pantallajuego.cpp pantallajuego.ui

    botonanimado.h botonanimado.cpp

    nodojugador.h nodojugador.cpp

    Resources.qrc
    cartaui.h cartaui.cpp
    carta.h carta.cpp
    carta.cpp
    nodo.h nodo.cpp
    listagenerica.h listagenerica.cpp


)

target_include_directories(Practica1-1s-2026-EDD PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Practica1-1s-2026-EDD
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS Practica1-1s-2026-EDD
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Practica1-1s-2026-EDD
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
