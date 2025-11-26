# 🚀 Guía Rápida de Deploy

## ✅ Backend en Render (5 minutos)

### Paso 1: Crear cuenta en Render
1. Ve a https://render.com
2. Click en "Get Started"
3. Conecta tu cuenta de GitHub

### Paso 2: Crear Web Service
1. Click en **"New +"** → **"Web Service"**
2. Conecta tu repositorio: `seo-onion/j_rust_compiler`
3. Dale permiso a Render para acceder al repo

### Paso 3: Configuración
Completa el formulario:

- **Name:** `j-rust-compiler` (o el nombre que quieras)
- **Region:** Oregon (US West) o el más cercano
- **Branch:** `master`
- **Root Directory:** *(déjalo vacío)*
- **Environment:** `Docker`
- **Plan:** `Free`

### Paso 4: Variables de entorno (Opcional)
En "Environment Variables" puedes agregar:
- `COMPILER_PATH` = `/app/compiler` (ya está configurado en render.yaml)

### Paso 5: Deploy
1. Click en **"Create Web Service"**
2. Render detectará automáticamente el `Dockerfile`
3. Espera 5-10 minutos mientras se construye
4. Verás logs en tiempo real

### Paso 6: Obtener URL
Cuando termine, Render te dará una URL como:
```
https://j-rust-compiler.onrender.com
```

**¡Guarda esta URL!** La necesitarás para el frontend.

### Verificar que funciona
1. Abre: `https://tu-app.onrender.com/health`
2. Deberías ver: `{"status":"ok","compiler":true}`

---

## ✅ Frontend en Vercel (3 minutos)

### Paso 1: Crear cuenta en Vercel
1. Ve a https://vercel.com
2. Click en "Sign Up"
3. Conecta tu cuenta de GitHub

### Paso 2: Importar proyecto
1. Click en **"Add New..."** → **"Project"**
2. Busca tu repo: `j_rust_compiler`
3. Click en **"Import"**

### Paso 3: Configuración del proyecto
En la pantalla de configuración:

**Framework Preset:** Create React App
**Root Directory:** `frontend`
**Build Command:** `npm run build` (autodetectado)
**Output Directory:** `build` (autodetectado)

### Paso 4: Variables de entorno
En **"Environment Variables"** agrega:

**Key:** `REACT_APP_API_URL`
**Value:** `https://j-rust-compiler.onrender.com` (tu URL de Render)

Aplica a: **Production, Preview, and Development**

### Paso 5: Deploy
1. Click en **"Deploy"**
2. Espera 2-3 minutos
3. Vercel construirá tu app

### Paso 6: ¡Listo!
Vercel te dará una URL como:
```
https://j-rust-compiler.vercel.app
```

¡Abre esa URL y tu compilador estará funcionando! 🎉

---

## 🔧 Troubleshooting

### Backend (Render)

**Error: Build failed**
- Verifica que el `Dockerfile` esté en la raíz del repo
- Revisa los logs de build en Render

**Error: Health check failed**
- Espera 1-2 minutos más, el servidor tarda en iniciar
- Verifica logs en Render Dashboard

**El compilador no funciona**
- Verifica que graphviz y gcc estén en el Dockerfile
- Revisa los logs cuando compiles código

### Frontend (Vercel)

**Error: Build failed**
- Verifica que `frontend/package.json` existe
- Asegúrate de que Root Directory = `frontend`

**No se conecta al backend**
- Verifica que `REACT_APP_API_URL` esté configurada
- Debe ser la URL completa de Render (con https://)
- Haz un nuevo deploy después de agregar la variable

**CORS errors**
- El backend ya tiene CORS configurado para `*`
- Si persiste, verifica que la URL del backend sea correcta

---

## 📝 Comandos útiles

### Actualizar backend (Render)
Render hace auto-deploy en cada push a master:
```bash
git add .
git commit -m "fix: mensaje"
git push origin master
```

### Actualizar frontend (Vercel)
Vercel también hace auto-deploy:
```bash
git add .
git commit -m "fix: mensaje"
git push origin master
```

### Ver logs en tiempo real
- **Render:** Dashboard → tu servicio → Logs
- **Vercel:** Dashboard → tu proyecto → Deployments → Click en el deployment

---

## 🎯 URLs Importantes

Después del deploy, guarda estas URLs:

- **Backend API:** https://TU-APP.onrender.com
- **Frontend:** https://TU-APP.vercel.app
- **Health Check:** https://TU-APP.onrender.com/health
- **API Docs:** https://TU-APP.onrender.com/docs

---

## 🚨 Importante

### Render Free Tier
- El servicio se "duerme" después de 15 minutos sin uso
- La primera petición después de dormir tarda ~30 segundos
- Esto es normal en el plan gratuito

### Vercel Free Tier
- 100GB bandwidth/mes
- Builds ilimitados
- Sin límite de peticiones

---

## ✅ Checklist Final

- [ ] Backend deployeado en Render
- [ ] Health check funciona: `/health`
- [ ] Frontend deployeado en Vercel
- [ ] Variable `REACT_APP_API_URL` configurada
- [ ] Probado compilar código desde el frontend
- [ ] Probado ver tokens, AST y ensamblador
- [ ] Probado ejecutar programa

¡Listo! Tu compilador está en producción 🎉
