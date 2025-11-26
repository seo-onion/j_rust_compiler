# Deploy en Fly.io 🚀

Fly.io es **gratis** y funciona muy similar a Render, pero sin las restricciones que causaban el segfault.

## 1. Instalar Fly CLI

### macOS/Linux:
```bash
curl -L https://fly.io/install.sh | sh
```

### Windows (PowerShell):
```powershell
pwsh -Command "iwr https://fly.io/install.ps1 -useb | iex"
```

Luego agrega al PATH (si no se agregó automáticamente):
```bash
# En ~/.bashrc o ~/.zshrc
export FLYCTL_INSTALL="$HOME/.fly"
export PATH="$FLYCTL_INSTALL/bin:$PATH"
```

## 2. Crear Cuenta en Fly.io

```bash
fly auth signup
```

O si ya tienes cuenta:
```bash
fly auth login
```

## 3. Crear la App (una sola vez)

```bash
# Asegúrate de estar en la raíz del proyecto
cd /ruta/a/proyecto_compis

# Crear app (usa el nombre que quieras)
fly apps create rust-compiler-api

# Crear volumen persistente para archivos temporales (gratis)
fly volumes create compiler_data --region mia --size 1
```

**Nota:** Cambia `mia` (Miami) a la región más cercana:
- `gru` - São Paulo, Brasil
- `scl` - Santiago, Chile
- `eze` - Buenos Aires, Argentina
- `mia` - Miami, USA

## 4. Deploy Inicial

```bash
fly deploy
```

⏱️ **Tiempo:** 5-10 minutos (primera vez)

## 5. Ver Tu App Funcionando

```bash
# Abrir en el navegador
fly open

# Ver logs en tiempo real
fly logs

# Ver status
fly status
```

## 6. URLs de tu App

Tu app estará disponible en:
- **URL principal:** `https://rust-compiler-api.fly.dev`
- **Health check:** `https://rust-compiler-api.fly.dev/health`
- **Documentación:** `https://rust-compiler-api.fly.dev/docs`

## 7. Configurar Variables de Entorno (si necesitas)

```bash
fly secrets set VARIABLE_NAME=valor
```

## 8. Actualizar el Código

Cada vez que hagas cambios:

```bash
git add .
git commit -m "tu mensaje"
git push origin master

# Deploy nueva versión
fly deploy
```

## 9. Escalar (si necesitas más recursos)

Fly.io free tier: 3 VMs de 256MB RAM

Si necesitas más RAM (para compilaciones grandes):

```bash
# Ver planes disponibles
fly scale show

# Escalar a 512MB (primeros $5/mes gratis con crédito)
fly scale memory 512

# Ver uso actual
fly dashboard
```

## 10. Monitoreo y Logs

```bash
# Ver logs en tiempo real
fly logs

# Ver métricas
fly dashboard

# SSH a la máquina (para debugging)
fly ssh console
```

## 11. Conectar Frontend

En tu frontend (Vercel), actualiza la variable de entorno:

```bash
REACT_APP_API_URL=https://rust-compiler-api.fly.dev
```

## Comandos Útiles

```bash
# Ver apps
fly apps list

# Detener app (deja de correr, ahorra recursos)
fly scale count 0

# Iniciar de nuevo
fly scale count 1

# Ver costos y uso
fly dashboard

# Eliminar app (si quieres empezar de cero)
fly apps destroy rust-compiler-api
```

## Troubleshooting

### Error de memoria (OOM)
Si ves "Out of Memory":
```bash
fly scale memory 512
```

### App no inicia
```bash
fly logs
# Revisa los logs para ver el error
```

### Rebuild desde cero
```bash
fly deploy --no-cache
```

### Health check falla
Verifica que el endpoint `/health` retorne 200:
```bash
curl https://rust-compiler-api.fly.dev/health
```

## Diferencias con Render

✅ **Ventajas de Fly.io:**
- Menos restricciones de seguridad (no debería tener el segfault)
- Más control sobre la configuración
- SSH directo a la máquina
- Free tier más flexible

✅ **El mismo Dockerfile funciona**
✅ **Sin cambios en el código**

## Costos

- **Free tier:** 3 VMs de 256MB RAM, 3GB storage, 160GB outbound
- **Si necesitas más:** ~$2-5/mes para 512MB RAM
- **Tarjeta requerida:** No (pero recomendada para escalar)

---

**¿Problemas?** Ejecuta `fly logs` y comparte el error.
