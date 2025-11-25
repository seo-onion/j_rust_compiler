# Backend FastAPI - Compilador de Rust

## Instalación

```bash
cd backend
python3 -m venv venv
source venv/bin/activate  # En Windows: venv\Scripts\activate
pip install -r requirements.txt
```

## Configuración

Antes de ejecutar el backend, asegúrate de:

1. Compilar tu compilador C++:
```bash
cd ..
g++ -o compiler main.cpp ast.cpp token.cpp scanner.cpp parser.cpp visitor.cpp ast_to_dot.cpp -std=c++17
```

2. El ejecutable `compiler` debe estar en el directorio raíz del proyecto

## Ejecutar el servidor

```bash
python main.py
```

O con uvicorn:
```bash
uvicorn main:app --reload --host 0.0.0.0 --port 8000
```

El servidor estará disponible en: `http://localhost:8000`

## Endpoints

### POST /compile
Compila el código y genera todos los archivos

**Body:**
```json
{
  "code": "fn main() { ... }"
}
```

**Response:**
```json
{
  "success": true,
  "message": "Compilación exitosa",
  "files": {
    "id": "abc123",
    "available": ["tokens", "dot", "png", "asm"]
  }
}
```

### GET /tokens/{compilation_id}
Retorna el archivo de tokens

### GET /ast/{compilation_id}
Retorna la imagen PNG del AST

### GET /dot/{compilation_id}
Retorna el archivo DOT del AST

### GET /asm/{compilation_id}
Retorna el código ensamblador

### GET /run/{compilation_id}
Compila y ejecuta el programa, retorna stdout/stderr

### GET /health
Verifica el estado del servidor

## Documentación interactiva

- Swagger UI: http://localhost:8000/docs
- ReDoc: http://localhost:8000/redoc
