# Setup Completo - Resumen

## Archivos Creados

### Compilador C++ - Generación de AST en DOT
✅ `ast_to_dot.h` - Header del generador DOT
✅ `ast_to_dot.cpp` - Implementación que recorre el AST y genera archivo .dot
✅ `main.cpp` - Modificado para generar .dot, .png, .tokens.txt y .s

### Backend FastAPI
✅ `backend/main.py` - API completa con todos los endpoints
✅ `backend/requirements.txt` - Dependencias Python
✅ `backend/start.sh` - Script para iniciar el servidor
✅ `backend/venv/` - Entorno virtual instalado y configurado
✅ `backend/.env.example` - Ejemplo de variables de entorno
✅ `backend/.gitignore` - Archivos a ignorar

### Frontend React
✅ `frontend/package.json` - Configuración del proyecto
✅ `frontend/src/App.js` - Componente principal con Monaco Editor
✅ `frontend/src/App.css` - Estilos completos
✅ `frontend/src/index.js` - Punto de entrada
✅ `frontend/src/index.css` - Estilos globales
✅ `frontend/public/index.html` - HTML base
✅ `frontend/.env.example` - Variables de entorno ejemplo
✅ `frontend/.env.local` - Variables para desarrollo local
✅ `frontend/.gitignore` - Archivos a ignorar
✅ `frontend/vercel.json` - Configuración para Vercel

### Deployment
✅ `Dockerfile` - Imagen Docker para Render
✅ `render.yaml` - Configuración automática para Render
✅ `.dockerignore` - Archivos a excluir del build
✅ `Makefile` - Compilación del proyecto C++
✅ `DEPLOYMENT.md` - Guía completa de deployment
✅ `README.md` - Documentación principal

---

## Inicio Rápido

### 1. Compilar el compilador C++

```bash
make
```

### 2. Iniciar Backend (ya con venv configurado)

```bash
cd backend
./start.sh
```

Backend disponible en: http://localhost:8000
Documentación: http://localhost:8000/docs

### 3. Iniciar Frontend

```bash
cd frontend
npm install  # Solo la primera vez
npm start
```

Frontend disponible en: http://localhost:3000

---

## Endpoints del Backend

### POST /compile
Recibe código, compila y genera todos los archivos

**Request:**
```json
{
  "code": "fn main() { print(42); }"
}
```

**Response:**
```json
{
  "success": true,
  "message": "Compilación exitosa",
  "files": {
    "id": "abc123def456",
    "available": ["tokens", "dot", "png", "asm"]
  }
}
```

### GET /tokens/{compilation_id}
Retorna el archivo `.tokens.txt`

### GET /ast/{compilation_id}
Retorna la imagen PNG del AST

### GET /dot/{compilation_id}
Retorna el archivo `.dot` del AST

### GET /asm/{compilation_id}
Retorna el código ensamblador `.s`

### GET /run/{compilation_id}
Compila el `.s` con GCC y ejecuta el binario

**Response:**
```json
{
  "success": true,
  "stdout": "42\n",
  "stderr": "",
  "returncode": 0
}
```

### GET /health
Verifica que el servidor funciona

---

## Funcionalidades del Frontend

1. **Editor Monaco** - Editor de código con sintaxis highlighting
2. **Compilación** - Envía código al backend y recibe resultados
3. **Tabs de visualización:**
   - **Tokens**: Muestra los tokens generados por el scanner
   - **AST**: Muestra la imagen gráfica del árbol sintáctico
   - **Ensamblador**: Muestra el código x86 generado
   - **Salida**: Muestra stdout/stderr de la ejecución
4. **Ejecución**: Compila y ejecuta el programa

---

## Deployment en Producción

### Backend en Render

**Opción Automática:**
1. Push a GitHub
2. En Render.com: New → Blueprint
3. Conectar repo → Apply
4. Listo (usa `render.yaml`)

**Opción Manual:**
1. New → Web Service
2. Environment: Docker
3. Agregar variable: `COMPILER_PATH=/app/compiler`
4. Deploy

### Frontend en Vercel

1. En Vercel.com: New Project
2. Conectar repo
3. Root Directory: `frontend`
4. Framework: Create React App
5. Variable de entorno: `REACT_APP_API_URL=https://tu-backend.onrender.com`
6. Deploy

---

## Archivos que genera el compilador

Al ejecutar `./compiler archivo.rs` se generan:

1. **archivo.tokens.txt** - Lista de tokens del scanner
2. **archivo.dot** - AST en formato DOT (Graphviz)
3. **archivo.png** - Imagen del AST (requiere graphviz instalado)
4. **archivo.s** - Código ensamblador x86

---

## Dependencias del Sistema

### Ubuntu/Debian
```bash
sudo apt-get install g++ make graphviz gcc python3 python3-venv nodejs npm
```

### macOS
```bash
brew install gcc make graphviz python node
```

---

## Estado del Proyecto

✅ Backend FastAPI completo y funcional
✅ Frontend React con Monaco Editor
✅ Generador de AST en formato DOT
✅ Main.cpp modificado para generar todos los archivos
✅ venv configurado e instalado
✅ Archivos de deployment para Render y Vercel
✅ Documentación completa
✅ CORS configurado
✅ Listo para deployment

---

## Próximos Pasos

1. **Testing local:**
   ```bash
   make && cd backend && ./start.sh
   ```

2. **Deploy backend:**
   - Push a GitHub
   - Conectar Render

3. **Deploy frontend:**
   - Conectar Vercel
   - Configurar variable `REACT_APP_API_URL`

---

## Troubleshooting

### Backend no inicia
```bash
cd backend
source venv/bin/activate
pip install -r requirements.txt
python main.py
```

### Frontend no compila
```bash
cd frontend
rm -rf node_modules package-lock.json
npm install
npm start
```

### Compilador no genera PNG
```bash
sudo apt-get install graphviz
```

### CORS errors
- Ya configurado en `main.py` con `allow_origins=["*"]`
- Verificar que la URL del backend sea correcta

---

## Contacto y Soporte

Para issues o preguntas, revisar:
- README.md
- DEPLOYMENT.md
- Logs del backend: `backend/temp_compilations/`
