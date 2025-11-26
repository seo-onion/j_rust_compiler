#!/bin/bash
# Script para configurar DigitalOcean Droplet para el compilador Rust

set -e  # Salir si hay algún error

echo "🚀 Instalando dependencias del sistema..."
sudo apt-get update
sudo apt-get install -y \
    python3 \
    python3-pip \
    python3-venv \
    g++ \
    make \
    graphviz \
    gcc \
    git \
    nginx

echo "📦 Compilando el compilador C++..."
make clean || true
make

echo "🐍 Configurando entorno virtual de Python..."
cd backend
python3 -m venv venv
source venv/bin/activate

echo "📚 Instalando dependencias de Python..."
pip install --upgrade pip
pip install -r requirements.txt

echo "✅ Setup completado!"
echo ""
echo "Para iniciar el servidor, ejecuta:"
echo "  ./start_production.sh"
