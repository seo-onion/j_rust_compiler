#!/bin/bash
# Script para iniciar el servidor en producción

set -e

# Activar entorno virtual
cd backend
source venv/bin/activate

# Verificar que el compilador existe
if [ ! -f "../compiler" ]; then
    echo "❌ Error: El compilador no está compilado"
    echo "Ejecuta primero: make"
    exit 1
fi

echo "🚀 Iniciando servidor en producción..."
echo "Servidor corriendo en http://0.0.0.0:8000"
echo "Presiona Ctrl+C para detener"

# Iniciar con uvicorn
python3 -m uvicorn main:app --host 0.0.0.0 --port 8000 --workers 2
