#!/bin/bash
echo "========================================="
echo "      🏥 HEALTH MONITOR WEB APP"
echo "========================================="
echo ""
echo "1. Compilando backend C++..."
cd ~/HealthMonitor

# Compilar servidor backend
if [ -f "backend/webserver.cpp" ]; then
    g++ -std=c++11 backend/webserver.cpp -o backend/server -lsqlite3
    if [ $? -eq 0 ]; then
        echo "✅ Backend compilado correctamente"
    else
        echo "⚠️  Error compilando backend"
    fi
else
    echo "❌ backend/webserver.cpp no encontrado"
fi

echo ""
echo "2. Iniciando servidor web..."
echo ""
echo "🌐 Abre tu navegador y visita:"
echo "   http://localhost:8080"
echo ""
echo "📁 Directorio frontend: ~/HealthMonitor/frontend"
echo ""
echo "Presiona Ctrl+C para detener el servidor"
echo "========================================="

# Iniciar servidor web Python
cd ~/HealthMonitor/frontend
python3 -m http.server 8080
