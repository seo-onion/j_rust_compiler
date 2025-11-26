# Guía de Uso del Depurador Interactivo

## Descripción

El compilador ahora incluye una herramienta interactiva para visualizar el estado de los registros y la pila de ejecución paso a paso durante la ejecución del código ensamblador.

## Características

### Backend (FastAPI)

1. **Endpoint `/debug/start/{compilation_id}`** (POST)
   - Inicia el modo debug para una compilación
   - Parsea el código ensamblador y extrae las instrucciones ejecutables
   - Retorna el estado inicial de registros y pila

2. **Endpoint `/debug/step`** (POST)
   - Ejecuta la simulación hasta el paso especificado
   - Retorna el estado actualizado de:
     - Registros (RAX, RBX, RCX, RDX, RSI, RDI, RBP, RSP, R8-R15, RIP)
     - Pila de ejecución
     - Instrucción actual
     - Memoria

### Frontend (React)

1. **Nueva pestaña "Depurador"**
   - Visualización de todos los registros en tiempo real
   - Visualización de la pila de ejecución (stack)
   - Lista de instrucciones con resaltado de la instrucción actual
   - Indicador de instrucción actual en ejecución

2. **Controles de navegación**
   - **← Anterior**: Retrocede un paso
   - **↻ Reiniciar**: Vuelve al estado inicial
   - **Siguiente →**: Avanza un paso
   - **■ Detener**: Detiene la sesión de debug

3. **Interfaz visual**
   - Grid de 3 columnas mostrando: Registros | Pila | Instrucciones
   - Resaltado azul en la instrucción actual
   - Contador de pasos (Paso X / Total)
   - Instrucción actual destacada en la parte inferior

## Cómo usar

1. **Compilar código**: Escribe tu código Rust y haz clic en "Compilar"
2. **Iniciar depurador**: Haz clic en el botón "Depurar" (naranja)
3. **Navegar paso a paso**:
   - Usa "Siguiente →" para avanzar instrucción por instrucción
   - Usa "← Anterior" para retroceder
   - Observa cómo cambian los valores de los registros y la pila
4. **Detener**: Haz clic en "■ Detener" cuando termines

## Instrucciones soportadas por el simulador

El simulador actualmente soporta:

- **Movimientos**: `movq` (inmediatos y entre registros)
- **Pila**: `pushq`, `popq`, `leave`
- **Aritmética**: `addq`, `subq`, `imulq`
- **Comparación**: `cmpq`
- **Saltos**: `je`, `jne`, `jmp`, `jl`, `jle`, `jg`, `jge`
- **Llamadas**: `call`, `ret`

## Ejemplo de uso

```rust
fn main() {
    let x: i64 = 10;
    let y: i64 = 20;
    let result: i64 = x + y;
    print(result);
}
```

1. Compila este código
2. Haz clic en "Depurar"
3. Ve paso a paso cómo:
   - Se cargan los valores 10 y 20 en registros
   - Se realiza la suma
   - El resultado se almacena
   - Se llama a printf

## Ventajas educativas

- **Visualización en tiempo real**: Ver exactamente qué hace cada instrucción
- **Comprensión de la pila**: Observar cómo crece y decrece con `push`/`pop`
- **Seguimiento de registros**: Entender el uso de registros x86-64
- **Debugging efectivo**: Identificar errores en la generación de código

## Notas técnicas

- El simulador es una interpretación simplificada del código ensamblador
- No ejecuta código real, simula la ejecución para propósitos educativos
- La pila comienza en la dirección 4096 (simulada)
- Los valores de memoria se rastrean de forma limitada
