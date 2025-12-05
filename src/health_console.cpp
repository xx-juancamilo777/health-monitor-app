// Health Monitor - Console Version
#include <iostream>
#include <sqlite3.h>

int main() {
    std::cout << "=================================\n";
    std::cout << "   HEALTH MONITOR CONSOLE\n";
    std::cout << "=================================\n";
    
    std::cout << "1. Probando C++... ";
    std::cout << "✅ OK\n";
    
    std::cout << "2. Probando SQLite... ";
    std::cout << sqlite3_libversion() << " ✅ OK\n";
    
    std::cout << "3. Creando base de datos... ";
    sqlite3 *db;
    if (sqlite3_open("health_data.db", &db) == SQLITE_OK) {
        std::cout << "✅ OK\n";
        sqlite3_close(db);
    } else {
        std::cout << "❌ ERROR\n";
    }
    
    std::cout << "\n🎉 Sistema listo!\n";
    std::cout << "Usuario: xx-juancamilo777\n";
    std::cout << "=================================\n";
    
    return 0;
}
