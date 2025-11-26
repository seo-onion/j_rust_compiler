# 📄 Informe del Compilador - LaTeX

Este directorio contiene el informe completo del proyecto en formato LaTeX.

## 🌐 Compilar Online en Overleaf

### Opción 1: Subir archivo directamente

1. Ve a https://www.overleaf.com/
2. Crea una cuenta gratuita o inicia sesión
3. Click en **"New Project"** → **"Upload Project"**
4. Sube el archivo `informe.tex`
5. Click en **"Recompile"**
6. ¡Listo! Descarga el PDF generado

### Opción 2: Crear proyecto nuevo

1. Ve a https://www.overleaf.com/
2. Click en **"New Project"** → **"Blank Project"**
3. Nombra el proyecto: "Compilador Rust"
4. Copia y pega el contenido de `informe.tex` en el editor
5. Click en **"Recompile"**
6. Descarga el PDF

## 📝 Otras opciones online

### LaTeX Online Compiler
- URL: https://latexeditor.lagrida.com/
- Simplemente pega el contenido y compila

### LaTeX Base
- URL: https://latexbase.com/
- Editor simple y rápido

### Papeeria
- URL: https://papeeria.com/
- Similar a Overleaf

## 🖥️ Compilar localmente

Si tienes LaTeX instalado en tu sistema:

```bash
cd informe
pdflatex informe.tex
bibtex informe
pdflatex informe.tex
pdflatex informe.tex
```

O con una sola pasada:
```bash
pdflatex informe.tex
```

## 📦 Paquetes necesarios

El documento usa los siguientes paquetes LaTeX:
- `inputenc` (UTF-8)
- `babel` (español)
- `amsmath`, `amsfonts`, `amssymb` (matemáticas)
- `graphicx` (imágenes)
- `listings` (código fuente)
- `xcolor` (colores)
- `hyperref` (enlaces)
- `geometry` (márgenes)
- `fancyhdr` (encabezados)
- `apacite` (referencias APA)

**Overleaf incluye todos estos paquetes por defecto**, no necesitas instalar nada.

## 📖 Estructura del documento

1. Portada
2. Tabla de contenidos
3. Introducción
4. Gramática
5. Parser
6. AST
7. Visitor (Typechecker)
8. Visitor (GenCode)
9. Arrays Dinámicas
10. Templates
11. Deploy y Producción
12. Conclusiones
13. Referencias bibliográficas

## ✏️ Personalización

Para personalizar el documento, edita estas líneas en `informe.tex`:

```latex
\author{Tu Nombre\\
\small Tu Universidad}
```

```latex
\rhead{Tu Título}
\lhead{Tu Curso}
```

## 🎨 Formato

- **Estilo**: APA 7ª edición
- **Fuente**: 12pt
- **Papel**: A4
- **Márgenes**: 2.5cm (todos los lados)
- **Interlineado**: Sencillo
- **Código**: Syntax highlighting para C++, Python, JavaScript

## 📥 Descargar PDF

Después de compilar en Overleaf:
1. Click en el ícono de descarga (flecha hacia abajo)
2. Selecciona "PDF"
3. El archivo se descargará como `informe.pdf`

## ⚠️ Solución de problemas

### Error de compilación
- Asegúrate de que todo el contenido esté en `informe.tex`
- Verifica que no haya caracteres especiales corruptos
- En Overleaf, cambia el compilador a "pdfLaTeX" en el menú

### Bibliografía no aparece
- Ejecuta BibTeX después de la primera compilación
- En Overleaf esto es automático

### Código no tiene colores
- El paquete `listings` con `xcolor` debe estar cargado
- Verifica que la configuración de `\lstset` esté presente

## 📞 Contacto

Para cualquier duda sobre el documento LaTeX o el proyecto:
- Email: sebastianhernandezminano@gmail.com

---

**Tip**: Overleaf permite colaboración en tiempo real, ideal para trabajo en equipo.
