#!/bin/bash

echo ">>> Criando pasta para guardar executável..."
mkdir build
echo ">>> Copiando imagem de teste para a pasta"
cp foto_img.jpg build
cd build
echo ">>> Preparando arquivos para compilação..."
cmake ../
echo ">>> Compilando"
make
echo ">>> Executando código..."
./ComedouroBot
echo ">>> FIM."