#!/bin/bash

echo "************************************"
echo " Compilando "
echo "************************************"

if [ ! -f "CMakeLists.txt" ]; then
    echo "Error: No se encontró CMakeLists.txt en esta carpeta."
    exit 1
fi

rm -rf build
mkdir build
cd build

echo "Generando proyecto con CMake..."

cmake -DCMAKE_PREFIX_PATH=$HOME/Qt/6.10.2/gcc_64 ..

if [ $? -ne 0 ]; then
    echo "Error en configuración CMake."
    exit 1
fi

echo "Compilando..."
cmake --build .

if [ $? -ne 0 ]; then
    echo "Error en compilación."
    exit 1
fi

echo "------------------------------------"
echo " Compilación exitosa."
echo "------------------------------------"