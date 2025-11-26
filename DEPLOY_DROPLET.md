# Deploy en DigitalOcean Droplet

## 1. Crear Droplet

1. Ve a https://cloud.digitalocean.com/droplets/new
2. Selecciona:
   - **Imagen:** Ubuntu 22.04 LTS
   - **Plan:** Basic ($6/mes - 1GB RAM, 1 CPU es suficiente)
   - **Datacenter:** El más cercano a tus usuarios
   - **Authentication:** SSH key (recomendado) o Password
3. Click en **Create Droplet**

## 2. Conectarse al Droplet

```bash
ssh root@TU_IP_DROPLET
```

## 3. Setup Inicial (una sola vez)

```bash
# Clonar el repositorio
git clone https://github.com/seo-onion/j_rust_compiler.git
cd j_rust_compiler

# Dar permisos de ejecución a los scripts
chmod +x setup_droplet.sh start_production.sh

# Ejecutar setup (instala dependencias y compila)
./setup_droplet.sh
```

⏱️ **Tiempo estimado:** 3-5 minutos

## 4. Iniciar el Servidor

```bash
# Opción 1: Ejecución simple (para testing)
./start_production.sh

# Opción 2: Con systemd (producción - se reinicia automáticamente)
sudo cp rust-compiler.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable rust-compiler
sudo systemctl start rust-compiler

# Ver logs
sudo journalctl -u rust-compiler -f
```

## 5. Configurar Firewall

```bash
# Permitir puerto 8000
sudo ufw allow 8000/tcp
sudo ufw allow 'OpenSSH'
sudo ufw enable
```

## 6. (Opcional) Configurar Nginx como Reverse Proxy

```bash
# Crear configuración de Nginx
sudo nano /etc/nginx/sites-available/rust-compiler
```

Pega esto:

```nginx
server {
    listen 80;
    server_name TU_DOMINIO_O_IP;

    location / {
        proxy_pass http://localhost:8000;
        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection 'upgrade';
        proxy_set_header Host $host;
        proxy_cache_bypass $http_upgrade;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
    }
}
```

```bash
# Activar la configuración
sudo ln -s /etc/nginx/sites-available/rust-compiler /etc/nginx/sites-enabled/
sudo nginx -t
sudo systemctl restart nginx

# Permitir HTTP
sudo ufw allow 'Nginx Full'
```

## 7. (Opcional) Configurar HTTPS con Let's Encrypt

```bash
sudo apt install certbot python3-certbot-nginx
sudo certbot --nginx -d tu-dominio.com
```

## 8. Actualizar el Código

```bash
cd j_rust_compiler
git pull origin master
make clean && make
sudo systemctl restart rust-compiler
```

## URLs de Acceso

- **Sin Nginx:** `http://TU_IP:8000`
- **Con Nginx:** `http://TU_IP` o `http://tu-dominio.com`
- **Documentación API:** `/docs`
- **Health check:** `/health`

## Troubleshooting

### Ver logs del servidor
```bash
sudo journalctl -u rust-compiler -f
```

### Verificar que el compilador está compilado
```bash
ls -la compiler
./compiler --version || echo "El compilador existe"
```

### Reiniciar el servidor
```bash
sudo systemctl restart rust-compiler
```

### Verificar que el puerto está abierto
```bash
sudo netstat -tulpn | grep :8000
```

## Frontend

Para conectar el frontend, actualiza la variable de entorno:

```bash
# En Vercel
REACT_APP_API_URL=http://TU_IP:8000
```

O con dominio y Nginx:

```bash
REACT_APP_API_URL=https://tu-dominio.com
```
