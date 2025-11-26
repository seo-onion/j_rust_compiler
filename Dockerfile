FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    g++ \
    make \
    graphviz \
    gcc \
    libc6-dev \
    binutils \
    && rm -rf /var/lib/apt/lists/*

# Verificar versiones
RUN gcc --version && ld --version

WORKDIR /app

COPY backend/requirements.txt ./backend/
RUN pip3 install --no-cache-dir -r backend/requirements.txt

# Romper cache de Docker en este punto para forzar COPY fresco
ARG CACHEBUST=1
RUN echo "Cache bust: $CACHEBUST"

COPY *.cpp *.h ./
COPY Makefile ./

# Limpiar cualquier archivo .o que pudiera existir
RUN rm -f *.o compiler || true

# Compilar desde cero
RUN make

# Verificar compilación
RUN ls -la compiler

COPY backend/ ./backend/

WORKDIR /app/backend

EXPOSE 8000

CMD ["python3", "-m", "uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000"]
