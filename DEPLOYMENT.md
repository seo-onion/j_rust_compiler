# Guía de Deployment

## Backend en Render

### Opción 1: Deployment Automático con Docker

1. **Preparar el repositorio:**
   - Asegúrate de que todos los archivos estén en el repositorio
   - El `Dockerfile` y `render.yaml` ya están configurados

2. **En Render.com:**
   - Crea una cuenta en https://render.com
   - Click en "New +" → "Blueprint"
   - Conecta tu repositorio de GitHub
   - Render detectará automáticamente el `render.yaml`
   - Click en "Apply"

3. **Variables de entorno:**
   - Se configuran automáticamente desde `render.yaml`
   - `COMPILER_PATH=/app/compiler`

### Opción 2: Deployment Manual

1. **En Render.com:**
   - Click en "New +" → "Web Service"
   - Conecta tu repositorio
   - Configuración:
     - **Environment:** Docker
     - **Docker Command:** (se usa el CMD del Dockerfile)
     - **Plan:** Free

2. **Variables de entorno:**
   ```
   COMPILER_PATH=/app/compiler
   ```

### Verificar deployment

Una vez deployeado, visita:
- `https://tu-app.onrender.com/health` - debe retornar `{"status":"ok","compiler":true}`
- `https://tu-app.onrender.com/docs` - Swagger UI interactiva

---

## Frontend en Vercel

### Deployment

1. **En Vercel.com:**
   - Crea una cuenta en https://vercel.com
   - Click en "Add New..." → "Project"
   - Conecta tu repositorio de GitHub
   - Configuración:
     - **Framework Preset:** Create React App
     - **Root Directory:** `frontend`
     - **Build Command:** `npm run build`
     - **Output Directory:** `build`

2. **Variables de entorno:**
   En Vercel, ve a Settings → Environment Variables:
   ```
   REACT_APP_API_URL=https://tu-backend.onrender.com
   ```

3. **Deploy:**
   - Click en "Deploy"
   - Espera a que termine el build
   - Tu app estará en `https://tu-proyecto.vercel.app`

### Deployment en v0.dev

v0.dev es principalmente para generar componentes. Para un deployment completo:

1. Usa el código generado en v0.dev
2. Copia los componentes a tu proyecto React
3. Deploy normalmente en Vercel o Netlify

---

## Testing Local

### Backend

```bash
# Compilar el compilador C++
make

# Iniciar backend
cd backend
./start.sh
```

El backend estará en `http://localhost:8000`

### Frontend

```bash
cd frontend
npm install
npm start
```

El frontend estará en `http://localhost:3000`

---

## Estructura de archivos generada

```
proyecto_compis/
├── backend/
│   ├── main.py                 # FastAPI app
│   ├── requirements.txt        # Dependencias Python
│   ├── start.sh               # Script de inicio local
│   ├── venv/                  # Entorno virtual (local)
│   └── temp_compilations/     # Archivos temporales
├── frontend/
│   ├── src/
│   │   ├── App.js            # Componente principal
│   │   └── App.css           # Estilos
│   ├── package.json          # Dependencias Node
│   └── vercel.json           # Config Vercel
├── Dockerfile                 # Para Render
├── render.yaml               # Config automática Render
├── Makefile                  # Compilar C++
├── main.cpp                  # Compilador principal
├── ast_to_dot.cpp            # Generador de DOT
└── *.cpp, *.h               # Resto del compilador
```

---

## Troubleshooting

### Backend no encuentra el compilador

Verifica que el `Dockerfile` compile correctamente:
```bash
docker build -t rust-compiler .
docker run -p 8000:8000 rust-compiler
```

### CORS errors en frontend

Asegúrate de que:
- El backend tiene CORS configurado (ya está en `main.py`)
- La URL del backend es correcta en `.env.local`

### Frontend no se conecta al backend

1. Verifica la variable `REACT_APP_API_URL` en Vercel
2. Verifica que el backend esté activo en Render
3. Chequea la consola del navegador para errores

---

## URLs importantes

- **Documentación API (local):** http://localhost:8000/docs
- **Health check:** `/health`
- **Backend en Render:** https://tu-app.onrender.com
- **Frontend en Vercel:** https://tu-proyecto.vercel.app
