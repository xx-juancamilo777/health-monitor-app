cmake_minimum_required(VERSION 3.16)
project(HealthMonitor VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Buscar Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql)

# Buscar SQLite
find_package(SQLite3 REQUIRED)

# Archivos fuente
set(SOURCES
    src/main.cpp
    src/DatabaseManager.cpp
)

# Archivos de cabecera
set(HEADERS
    include/DatabaseManager.h
)

# Crear ejecutable
add_executable(HealthMonitor ${SOURCES} ${HEADERS})

# Incluir directorios
target_include_directories(HealthMonitor PRIVATE include)

# Enlazar bibliotecas
target_link_libraries(HealthMonitor
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
    SQLite::SQLite3
)
