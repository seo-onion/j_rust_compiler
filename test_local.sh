#!/bin/bash

echo "==================================="
echo "Test Local - Compilador de Rust"
echo "==================================="
echo ""

# Compilar el compilador
echo "1. Compilando el compilador C++..."
make clean
make

if [ $? -ne 0 ]; then
    echo "❌ Error al compilar el compilador"
    exit 1
fi

echo "✅ Compilador creado exitosamente"
echo ""

# Verificar backend
echo "2. Verificando backend..."
if [ ! -d "backend/venv" ]; then
    echo "❌ venv no encontrado. Ejecuta: cd backend && python3 -m venv venv && source venv/bin/activate && pip install -r requirements.txt"
    exit 1
fi

echo "✅ Backend configurado"
echo ""

# Verificar frontend
echo "3. Verificando frontend..."
if [ ! -d "frontend/node_modules" ]; then
    echo "⚠️  node_modules no encontrado. Ejecutando npm install..."
    cd frontend
    npm install
    cd ..
fi

echo "✅ Frontend configurado"
echo ""

# Crear archivo de prueba
echo "4. Creando archivo de prueba..."
cat > test.rs << 'EOF'
fn main() {
    let x: int = 10;
    let y: int = 20;
    let result: int = x + y;
    print(result);
}
EOF

echo "✅ Archivo test.rs creado"
echo ""

# Compilar archivo de prueba
echo "5. Compilando test.rs..."
./compiler test.rs

if [ $? -eq 0 ]; then
    echo "✅ Compilación exitosa"
    echo ""
    echo "Archivos generados:"
    ls -lh test.tokens.txt test.dot test.png test.s 2>/dev/null | awk '{print "  - " $9 " (" $5 ")"}'
else
    echo "❌ Error en la compilación"
    exit 1
fi

echo ""
echo "==================================="
echo "✅ Todo funcionando correctamente!"
echo "==================================="
echo ""
echo "Para iniciar el servidor completo:"
echo ""
echo "Terminal 1 (Backend):"
echo "  cd backend && ./start.sh"
echo ""
echo "Terminal 2 (Frontend):"
echo "  cd frontend && npm start"
echo ""
echo "Luego abre: http://localhost:3000"
echo ""
