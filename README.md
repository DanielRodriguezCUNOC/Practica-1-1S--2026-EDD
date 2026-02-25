# UNO FLIP — Práctica 1 · EDD 2026

## Requisitos previos

### Linux
| Herramienta | Versión mínima | Instalación Fedora |
|---|---|---|
| Qt | 6.x / 5.15 | `sudo dnf install qt6-devel o sudo dnf install qt5-devel` |
| g++ | 11+ | `sudo dnf install gcc-c++` |
| make | cualquiera | `sudo dnf install make` |
| qmake | incluido con Qt | `sudo dnf install qt6-qtbase-devel o sudo dnf install qt5-qtbase-devel` |

### Windows
| Herramienta | Descarga |
|---|---|
| Qt (MinGW) | https://www.qt.io/download-open-source |
| MinGW (g++ + mingw32-make) | incluido en el instalador de Qt |

> ⚠️ En Windows agrega `C:\Qt\<versión>\mingw_64\bin` y
> `C:\Qt\Tools\mingw<ver>\bin` al **PATH** del sistema.

---

## Compilar en Linux

```bash
# 1. Clonar o descomprimir el proyecto
cd Practica-1-1S--2026-EDD

# 2. Compilar
make

# 3. Ejecutar
make run

# limpiar artefactos
make clean
```

Si `qmake` no está en el PATH, especifícalo:

```bash
QMAKE=/ruta/a/qmake make
```

---

## Compilar en Windows (PowerShell - CMD)

```bat
:: 1. Abrir la carpeta del proyecto
cd Practica-1-1S--2026-EDD

:: 2. Compilar
mingw32-make

:: 3. Ejecutar
mingw32-make run

:: limpiar
mingw32-make clean
```

> Si `mingw32-make` no se reconoce, usa la terminal
> **Qt MinGW** que instala el asistente de Qt.

---

## Compilar desde Qt Creator (ambos SO)

1. Abrir **Qt Creator**.
2. `Archivo → Abrir proyecto` → seleccionar `Practica-1-1S--2026-EDD.pro`.
3. Configurar el kit (`Desktop Qt 6.x MinGW` en Windows / `Desktop Qt 6.x GCC` en Linux).
4. `Ctrl + B` para compilar.
5. `Ctrl + R` para ejecutar.

---

## Estructura del proyecto

```
Practica1-1s-2026-EDD/
├── assets/               # Imágenes de las cartas
├── build/                # Artefactos de compilación (Aqui se crea el archivo ejecutable con el nombre del proyecto)
├── *.h / *.cpp           # Código fuente
├── *.ui                  # Interfaz Grafica
├── Practica1-1s-2026-EDD.pro
├── Makefile
└── README.md
```

---

## Notas

- El proyecto usa **Qt Widgets** y **QGraphicsScene**.
- No requiere librerías externas adicionales.