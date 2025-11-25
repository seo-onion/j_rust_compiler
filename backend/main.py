from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import FileResponse, JSONResponse
from pydantic import BaseModel
import subprocess
import os
import shutil
from pathlib import Path

app = FastAPI(title="Rust Compiler API", version="1.0.0")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

TEMP_DIR = Path("temp_compilations")
TEMP_DIR.mkdir(exist_ok=True)

BASE_DIR = Path(__file__).resolve().parent
COMPILER_PATH = os.getenv("COMPILER_PATH", str(BASE_DIR.parent / "compiler"))

class CompileRequest(BaseModel):
    code: str

class CompileResponse(BaseModel):
    success: bool
    message: str
    files: dict

@app.post("/compile", response_model=CompileResponse)
async def compile_code(request: CompileRequest):
    """
    Recibe código, lo compila y genera:
    - archivo.tokens.txt
    - archivo.dot
    - archivo.png (AST)
    - archivo.s (ensamblador)
    """
    # Crear directorio temporal único para esta compilación
    temp_id = os.urandom(8).hex()
    temp_path = TEMP_DIR / temp_id
    temp_path.mkdir(exist_ok=True)

    input_file = temp_path / "input.rs"

    try:
        # Guardar código en archivo temporal
        with open(input_file, "w", encoding="utf-8") as f:
            f.write(request.code)

        # Ejecutar compilador (usa ruta absoluta del archivo)
        result = subprocess.run(
            [COMPILER_PATH, str(input_file.resolve())],
            cwd=str(temp_path.resolve()),
            capture_output=True,
            text=True,
            timeout=30
        )

        # Verificar archivos generados
        base_name = "input"
        files_generated = {
            "tokens": f"{base_name}_tokens.txt",  # El scanner usa guion bajo
            "dot": f"{base_name}.dot",
            "png": f"{base_name}.png",
            "asm": f"{base_name}.s"
        }

        missing_files = []
        for key, filename in files_generated.items():
            if not (temp_path / filename).exists():
                missing_files.append(filename)

        if result.returncode != 0:
            # Código -6 indica SIGABRT (crash del compilador)
            if result.returncode == -6:
                error_msg = "Error de sintaxis en el código\n\n"
                if result.stdout:
                    error_msg += f"{result.stdout}\n"
                if "Error en parseF" in result.stdout:
                    error_msg += "\nEl parser encontró un token inesperado. Verifica la sintaxis de tu código."
            else:
                error_msg = f"Error de compilación (código: {result.returncode})\n"
                error_msg += f"STDOUT:\n{result.stdout}\n"
                error_msg += f"STDERR:\n{result.stderr}\n"

            return CompileResponse(
                success=False,
                message=error_msg,
                files={}
            )

        return CompileResponse(
            success=True,
            message="Compilación exitosa",
            files={
                "id": temp_id,
                "available": [k for k, v in files_generated.items() if (temp_path / v).exists()]
            }
        )

    except subprocess.TimeoutExpired:
        return CompileResponse(
            success=False,
            message="Timeout: La compilación tomó demasiado tiempo",
            files={}
        )
    except Exception as e:
        return CompileResponse(
            success=False,
            message=f"Error inesperado: {str(e)}",
            files={}
        )

@app.get("/tokens/{compilation_id}")
async def get_tokens(compilation_id: str):
    """Retorna el archivo _tokens.txt"""
    file_path = TEMP_DIR / compilation_id / "input_tokens.txt"

    if not file_path.exists():
        raise HTTPException(status_code=404, detail="Archivo de tokens no encontrado")

    return FileResponse(
        path=file_path,
        media_type="text/plain",
        filename="tokens.txt"
    )

@app.get("/ast/{compilation_id}")
async def get_ast_image(compilation_id: str):
    """Retorna la imagen PNG del AST"""
    file_path = TEMP_DIR / compilation_id / "input.png"

    if not file_path.exists():
        raise HTTPException(status_code=404, detail="Imagen del AST no encontrada")

    return FileResponse(
        path=file_path,
        media_type="image/png",
        filename="ast.png"
    )

@app.get("/dot/{compilation_id}")
async def get_dot(compilation_id: str):
    """Retorna el archivo .dot del AST"""
    file_path = TEMP_DIR / compilation_id / "input.dot"

    if not file_path.exists():
        raise HTTPException(status_code=404, detail="Archivo DOT no encontrado")

    return FileResponse(
        path=file_path,
        media_type="text/plain",
        filename="ast.dot"
    )

@app.get("/asm/{compilation_id}")
async def get_assembly(compilation_id: str):
    """Retorna el archivo .s de ensamblador"""
    file_path = TEMP_DIR / compilation_id / "input.s"

    if not file_path.exists():
        raise HTTPException(status_code=404, detail="Archivo de ensamblador no encontrado")

    return FileResponse(
        path=file_path,
        media_type="text/plain",
        filename="output.s"
    )

@app.get("/run/{compilation_id}")
async def run_program(compilation_id: str):
    """
    Compila el archivo .s con GCC y ejecuta el binario resultante.
    Retorna stdout y stderr.
    """
    asm_file = TEMP_DIR / compilation_id / "input.s"

    if not asm_file.exists():
        raise HTTPException(status_code=404, detail="Archivo de ensamblador no encontrado")

    temp_path = TEMP_DIR / compilation_id
    binary_file = temp_path / "output"

    try:
        # Preprocesar archivo .s para eliminar etiquetas duplicadas (optimización)
        with open(asm_file, 'r') as f:
            lines = f.readlines()

        seen_labels = set()
        processed_lines = []
        for line in lines:
            stripped = line.strip()
            # Si es una etiqueta while_X: duplicada, convertirla en comentario
            if stripped.endswith(':') and stripped.startswith('while_'):
                if stripped in seen_labels:
                    # Etiqueta duplicada - es la optimización, comentarla
                    processed_lines.append('# ' + line)
                    continue
                seen_labels.add(stripped)
            processed_lines.append(line)

        # Guardar archivo procesado
        with open(asm_file, 'w') as f:
            f.writelines(processed_lines)

        # Compilar .s con GCC
        compile_result = subprocess.run(
            ["gcc", str(asm_file), "-o", str(binary_file), "-no-pie"],
            capture_output=True,
            text=True,
            timeout=15
        )

        if compile_result.returncode != 0:
            return JSONResponse(
                status_code=400,
                content={
                    "success": False,
                    "stdout": "",
                    "stderr": f"Error al compilar con GCC:\n{compile_result.stderr}"
                }
            )

        # Ejecutar binario
        run_result = subprocess.run(
            [str(binary_file)],
            capture_output=True,
            text=True,
            timeout=10
        )

        return {
            "success": True,
            "stdout": run_result.stdout,
            "stderr": run_result.stderr,
            "returncode": run_result.returncode
        }

    except subprocess.TimeoutExpired:
        return JSONResponse(
            status_code=400,
            content={
                "success": False,
                "stdout": "",
                "stderr": "Timeout: El programa tomó demasiado tiempo en ejecutarse"
            }
        )
    except Exception as e:
        return JSONResponse(
            status_code=500,
            content={
                "success": False,
                "stdout": "",
                "stderr": f"Error inesperado: {str(e)}"
            }
        )

@app.delete("/cleanup/{compilation_id}")
async def cleanup_files(compilation_id: str):
    """Elimina los archivos temporales de una compilación"""
    temp_path = TEMP_DIR / compilation_id

    if temp_path.exists():
        shutil.rmtree(temp_path)
        return {"message": "Archivos eliminados"}

    raise HTTPException(status_code=404, detail="Compilación no encontrada")

@app.get("/health")
async def health_check():
    """Verifica que el servidor está funcionando"""
    return {"status": "ok", "compiler": os.path.exists(COMPILER_PATH)}

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
