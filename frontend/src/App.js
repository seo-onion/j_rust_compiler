import React, { useState } from 'react';
import Editor from '@monaco-editor/react';
import axios from 'axios';
import './App.css';
const API_URL = process.env.REACT_APP_API_URL || 'http://localhost:8000';

const DEFAULT_CODE = `fn main() {
    let (a,i,s,j);
    s="pepito";
    a=[["aaa","bbb","ccc"],["gloria","andre",s]];
    s="juancito";
    a=[["aaa","bbb","ccc"],["gloria","andre",s],["1","2","3"]];
    a[0][0]="xddddddd";
    j=0;
    i=0;
    while i<3 {
      j=0;
      while j<3{
            print!("{}",a[i][j]);
            j=j+1;
      }
      i=i+1;
    }
    a[0]=["e","f","g"];
    i=0;
    while i<3{
        print!("{}",a[0][i]);
        i=i+1;
    }
}

}`;

function App() {
  const logo = "https://res.cloudinary.com/devuyzizr/image/upload/v1764066333/ChatGPT_Image_25_nov_2025_04_17_03_vucqd3.png"
  const [code, setCode] = useState(DEFAULT_CODE);
  const [compilationId, setCompilationId] = useState(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');
  const [tokens, setTokens] = useState('');
  const [assembly, setAssembly] = useState('');
  const [astImage, setAstImage] = useState(null);
  const [output, setOutput] = useState('');
  const [activeTab, setActiveTab] = useState('tokens');
  const [debugState, setDebugState] = useState(null);
  const [debugInstructions, setDebugInstructions] = useState([]);
  const [currentStep, setCurrentStep] = useState(0);
  const [isDebugging, setIsDebugging] = useState(false);

  const handleCompile = async () => {
    setLoading(true);
    setError('');
    setTokens('');
    setAssembly('');
    setAstImage(null);
    setOutput('');
    setCompilationId(null);

    try {
      const response = await axios.post(`${API_URL}/compile`, { code });

      if (response.data.success) {
        const id = response.data.files.id;
        setCompilationId(id);

        // Cargar tokens
        try {
          const tokensRes = await axios.get(`${API_URL}/tokens/${id}`);
          setTokens(tokensRes.data);
        } catch (e) {
          console.log('No se pudieron cargar los tokens');
        }

        // Cargar ensamblador
        try {
          const asmRes = await axios.get(`${API_URL}/asm/${id}`);
          setAssembly(asmRes.data);
        } catch (e) {
          console.log('No se pudo cargar el ensamblador');
        }

        // Cargar imagen AST
        try {
          setAstImage(`${API_URL}/ast/${id}`);
        } catch (e) {
          console.log('No se pudo cargar la imagen del AST');
        }

      } else {
        setError(response.data.message);
      }
    } catch (err) {
      setError(`Error de conexión: ${err.message}`);
    } finally {
      setLoading(false);
    }
  };

  const handleRun = async () => {
    if (!compilationId) {
      setError('Primero debes compilar el código');
      return;
    }

    setLoading(true);
    setError('');

    try {
      const response = await axios.get(`${API_URL}/run/${compilationId}`);

      if (response.data.success) {
        setOutput(`stdout:\n${response.data.stdout}\n\nstderr:\n${response.data.stderr}`);
      } else {
        setError(response.data.stderr);
      }
    } catch (err) {
      setError(`Error al ejecutar: ${err.message}`);
    } finally {
      setLoading(false);
    }
  };

  const handleStartDebug = async () => {
    if (!compilationId) {
      setError('Primero debes compilar el código');
      return;
    }

    setLoading(true);
    setError('');

    try {
      const response = await axios.post(`${API_URL}/debug/start/${compilationId}`);

      setDebugInstructions(response.data.instructions);
      setDebugState(response.data.initial_state);
      setCurrentStep(0);
      setIsDebugging(true);
      setActiveTab('debugger');
    } catch (err) {
      setError(`Error al iniciar debug: ${err.message}`);
    } finally {
      setLoading(false);
    }
  };

  const handleDebugStep = async (step) => {
    if (!compilationId || !isDebugging) return;

    try {
      const response = await axios.post(`${API_URL}/debug/step`, {
        compilation_id: compilationId,
        step: step
      });

      setDebugState(response.data);
      setCurrentStep(step);
    } catch (err) {
      setError(`Error en debug step: ${err.message}`);
    }
  };

  const handleNextStep = () => {
    if (currentStep < debugInstructions.length - 1) {
      handleDebugStep(currentStep + 1);
    }
  };

  const handlePrevStep = () => {
    if (currentStep > 0) {
      handleDebugStep(currentStep - 1);
    }
  };

  const handleResetDebug = () => {
    setCurrentStep(0);
    handleDebugStep(0);
  };

  const handleStopDebug = () => {
    setIsDebugging(false);
    setDebugState(null);
    setDebugInstructions([]);
    setCurrentStep(0);
  };

  return (
    <div className="app">
      <header className="header">
        <div style={{display: "flex", alignItems: "center", gap: "20px"}}> <img src={logo} style={{ transform: "scale(1.6)"}} width={120} /> <h1>J Rust Compiler</h1></div> 

        <p>Compilador de Rust en c++</p>
        <div className="header-buttons">
          <button onClick={handleCompile} disabled={loading} className="btn-compile">
            {loading ? 'Compilando...' : 'Compilar'}
          </button>
          <button onClick={handleRun} disabled={loading || !compilationId} className="btn-run">
            Ejecutar
          </button>
          <button onClick={handleStartDebug} disabled={loading || !compilationId} className="btn-debug">
            Depurar
          </button>
        </div>
      </header>

      <div className="main-container">
        <div className="editor-section">
          <h3>Editor de Código</h3>
          <Editor
            height="calc(100% - 40px)"
            defaultLanguage="rust"
            theme="vs-dark"
            value={code}
            onChange={(value) => setCode(value)}
            options={{
              minimap: { enabled: false },
              fontSize: 14,
              lineNumbers: 'on',
              scrollBeyondLastLine: false,
              automaticLayout: true,
            }}
          />
        </div>

        <div className="output-section">
          <div className="tabs">
            <button
              className={activeTab === 'tokens' ? 'tab active' : 'tab'}
              onClick={() => setActiveTab('tokens')}
            >
              Tokens
            </button>
            <button
              className={activeTab === 'ast' ? 'tab active' : 'tab'}
              onClick={() => setActiveTab('ast')}
            >
              AST
            </button>
            <button
              className={activeTab === 'assembly' ? 'tab active' : 'tab'}
              onClick={() => setActiveTab('assembly')}
            >
              Ensamblador
            </button>
            <button
              className={activeTab === 'output' ? 'tab active' : 'tab'}
              onClick={() => setActiveTab('output')}
            >
              Salida
            </button>
            <button
              className={activeTab === 'debugger' ? 'tab active' : 'tab'}
              onClick={() => setActiveTab('debugger')}
            >
              Depurador
            </button>
          </div>

          <div className="output-content">
            {error && (
              <div className="error-message">
                <strong>Error:</strong>
                <pre>{error}</pre>
              </div>
            )}

            {activeTab === 'tokens' && (
              <pre className="output-text">{tokens || 'Compila el código para ver los tokens'}</pre>
            )}

            {activeTab === 'ast' && (
              <div className="ast-container">
                {astImage ? (
                  <img src={astImage} alt="AST" className="ast-image" />
                ) : (
                  <p>Compila el código para ver el AST</p>
                )}
              </div>
            )}

            {activeTab === 'assembly' && (
              <pre className="output-text">{assembly || 'Compila el código para ver el ensamblador'}</pre>
            )}

            {activeTab === 'output' && (
              <pre className="output-text">{output || 'Ejecuta el programa para ver la salida'}</pre>
            )}

            {activeTab === 'debugger' && (
              <div className="debugger-container">
                {!isDebugging ? (
                  <p className="text-white">Haz clic en "Depurar" para iniciar el modo debug</p>
                ) : (
                  <div className="debug-view">
                    <div className="debug-controls">
                      <button onClick={handlePrevStep} disabled={currentStep === 0} className="debug-btn">
                        ← Anterior
                      </button>
                      <button onClick={handleResetDebug} className="debug-btn">
                        ↻ Reiniciar
                      </button>
                      <button onClick={handleNextStep} disabled={currentStep >= debugInstructions.length - 1} className="debug-btn">
                        Siguiente →
                      </button>
                      <button onClick={handleStopDebug} className="debug-btn-stop">
                        ■ Detener
                      </button>
                      <span className="debug-step-info text-white">
                        Paso: {currentStep + 1} / {debugInstructions.length}
                      </span>
                    </div>

                    <div className="debug-panels">
                      <div className="debug-panel">
                        <h4 className="text-white">Registros</h4>
                        <div className="registers-grid">
                          {debugState && Object.entries(debugState.registers).map(([reg, value]) => (
                            <div key={reg} className="register-item">
                              <span className="register-name text-white">{reg.toUpperCase()}:</span>
                              <span className="register-value text-white">{value}</span>
                            </div>
                          ))}
                        </div>
                      </div>

                      <div className="debug-panel">
                        <h4 className="text-white">Pila de Ejecución</h4>
                        <div className="stack-view">
                          {debugState && debugState.stack.length > 0 ? (
                            debugState.stack.slice().reverse().map((value, idx) => (
                              <div key={idx} className="stack-item">
                                <span className="stack-address text-white">[{debugState.stack.length - idx - 1}]</span>
                                <span className="stack-value text-white">{value}</span>
                              </div>
                            ))
                          ) : (
                            <p className="text-white">Pila vacía</p>
                          )}
                        </div>
                      </div>

                      <div className="debug-panel debug-instructions">
                        <h4 className="text-white">Instrucciones</h4>
                        <div className="instructions-list">
                          {debugInstructions.map((inst, idx) => (
                            <div
                              key={idx}
                              className={`instruction-line ${idx === currentStep ? 'current' : ''}`}
                            >
                              <span className="instruction-number text-white">{idx + 1}</span>
                              <span className="instruction-text text-white">{inst}</span>
                            </div>
                          ))}
                        </div>
                      </div>
                    </div>

                    {debugState && (
                      <div className="current-instruction-display">
                        <strong className="text-white">Instrucción actual: </strong>
                        <code className="text-white">{debugState.current_instruction}</code>
                      </div>
                    )}
                  </div>
                )}
              </div>
            )}
          </div>
        </div>
      </div>
    </div>
  );
}

export default App;
