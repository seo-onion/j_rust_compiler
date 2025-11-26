FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    g++ \
    make \
    graphviz \
    gcc \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY backend/requirements.txt ./backend/
RUN pip3 install --no-cache-dir -r backend/requirements.txt

COPY *.cpp *.h ./
COPY Makefile ./

# Forzar recompilación de parser.cpp
RUN touch parser.cpp
RUN make

COPY backend/ ./backend/

WORKDIR /app/backend

EXPOSE 8000

CMD ["python3", "-m", "uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000"]
