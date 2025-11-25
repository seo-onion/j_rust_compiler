# Compilador de Rust en C++

Compilador básico de Rust implementado en C++ con frontend web en React y backend API en FastAPI.

## Características

- **Scanner**: Tokenización del código fuente
- **Parser**: Análisis sintáctico y generación de AST
- **Type Checker**: Verificación de tipos
- **Code Generator**: Generación de código ensamblador x86
- **Visualización AST**: Generación automática de gráficos del AST usando Graphviz
- **API REST**: Backend FastAPI para compilación remota
- **Frontend Web**: Editor Monaco con visualización de resultados

## Componentes

### Compilador C++
- `main.cpp` - Punto de entrada
- `scanner.{h,cpp}` - Análisis léxico
- `parser.{h,cpp}` - Análisis sintáctico
- `ast.{h,cpp}` - Definiciones del AST
- `visitor.{h,cpp}` - Type checker y generador de código
- `ast_to_dot.{h,cpp}` - Generador de grafos DOT
- `token.{h,cpp}` - Definiciones de tokens
- `environment.h` - Manejo de entornos de variables

### Backend API
- FastAPI con endpoints para:
  - Compilación de código
  - Obtención de tokens
  - Visualización del AST
  - Código ensamblador
  - Ejecución de programas

### Frontend Web
- React con Monaco Editor
- Visualización de:
  - Tokens generados
  - AST en formato gráfico
  - Código ensamblador x86
  - Salida de ejecución

## Instalación Local

### Requisitos

- g++ (C++17 o superior)
- Python 3.8+
- Node.js 16+
- Graphviz
- GCC (para ejecutar ensamblador)
- Make

#### Instalar dependencias (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install g++ make graphviz gcc python3 python3-venv nodejs npm
```

### Compilar el compilador

```bash
make
```

Esto genera el ejecutable `compiler`.

### Configurar backend

```bash
cd backend
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```

### Configurar frontend

```bash
cd frontend
npm install
```

## Uso Local

### 1. Iniciar el backend

```bash
cd backend
./start.sh
```

El backend estará disponible en `http://localhost:8000`

### 2. Iniciar el frontend

En otra terminal:

```bash
cd frontend
npm start
```

El frontend se abrirá automáticamente en `http://localhost:3000`

### 3. Usar el compilador directamente

```bash
./compiler archivo.rs
```

Esto genera:
- `archivo.tokens.txt` - Tokens del scanner
- `archivo.dot` - AST en formato DOT
- `archivo.png` - Imagen del AST
- `archivo.s` - Código ensamblador x86

## API Endpoints

### POST `/compile`
Compila código y genera todos los archivos

```bash
curl -X POST http://localhost:8000/compile \
  -H "Content-Type: application/json" \
  -d '{"code": "fn main() { print(42); }"}'
```

### GET `/tokens/{compilation_id}`
Obtiene los tokens generados

### GET `/ast/{compilation_id}`
Obtiene la imagen PNG del AST

### GET `/asm/{compilation_id}`
Obtiene el código ensamblador

### GET `/run/{compilation_id}`
Compila y ejecuta el programa, retorna stdout/stderr

### GET `/health`
Verifica el estado del servidor

## Deployment

Ver [DEPLOYMENT.md](./DEPLOYMENT.md) para instrucciones detalladas de deployment en Render y Vercel.

## Estructura del Proyecto

```
.
├── backend/              # API FastAPI
│   ├── main.py
│   ├── requirements.txt
│   └── venv/
├── frontend/            # React app
│   ├── src/
│   ├── public/
│   └── package.json
├── *.cpp, *.h          # Compilador C++
├── Makefile            # Build del compilador
├── Dockerfile          # Para deployment
└── README.md
```

## Ejemplo de Código

```rust
fn main() {
    let x: int = 10;
    let y: int = 20;
    let result: int = x + y;
    print(result);
}
```

## Licencia

Este proyecto es para fines educativos.
