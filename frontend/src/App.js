import React, { useState } from 'react';
import Editor from '@monaco-editor/react';
import axios from 'axios';
import './App.css';
const API_URL = process.env.REACT_APP_API_URL || 'http://localhost:8000';

const DEFAULT_CODE = `fn main() {
    let x: int = 10;
    let y: int = 20;
    let result: int = x + y;
    print(result);
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
          </div>
        </div>
      </div>
    </div>
  );
}

export default App;
