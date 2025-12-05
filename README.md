# 🏥 Health Monitor App

Aplicación de escritorio en C++/Qt para registrar y analizar indicadores básicos de salud.

## ✨ Características
- ✅ Registro de peso, presión arterial y niveles de glucosa
- ✅ Almacenamiento seguro en SQLite
- ✅ Interfaz gráfica con Qt6
- ✅ Análisis de tendencias
- ✅ Exportación a CSV

## 🛠️ Requisitos Técnicos
- **Qt 6.0** o superior
- **SQLite3**
- **CMake 3.16+**
- **Compilador C++17** (g++ 8+, clang 7+, MSVC 2019+)

## 🚀 Instalación y Compilación

```bash
# Clonar repositorio
git clone https://github.com/xx-juancamilo777/health-monitor-app.git
cd health-monitor-app

# Compilar
mkdir build && cd build
cmake ..
make

# Ejecutar
./HealthMonitor
