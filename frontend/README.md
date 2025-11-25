# Frontend - Compilador de Rust

Frontend React para el compilador de Rust hecho en C++.

## Instalación

```bash
cd frontend
npm install
```

## Configuración

Asegúrate de que el backend FastAPI esté ejecutándose en `http://localhost:8000`

## Ejecutar

```bash
npm start
```

La aplicación se abrirá automáticamente en `http://localhost:3000`

## Características

- **Editor Monaco**: Editor de código con sintaxis de Rust
- **Compilación en tiempo real**: Compila tu código y visualiza los resultados
- **Visualización de Tokens**: Ve los tokens generados por el scanner
- **Visualización del AST**: Imagen gráfica del árbol de sintaxis abstracta
- **Código Ensamblador**: Ve el código x86 generado
- **Ejecución**: Compila y ejecuta el programa, viendo stdout y stderr

## Estructura

```
frontend/
├── public/
│   └── index.html
├── src/
│   ├── App.js          # Componente principal
│   ├── App.css         # Estilos
│   ├── index.js        # Punto de entrada
│   └── index.css       # Estilos globales
├── package.json
└── README.md
```

## Build para producción

```bash
npm run build
```

Esto generará una carpeta `build/` con los archivos estáticos optimizados.
