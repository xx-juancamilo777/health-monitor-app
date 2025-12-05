#include <iostream>
#include <sqlite3.h>
#include <sstream>
#include <map>

std::string getHealthRecords() {
    sqlite3 *db;
    std::stringstream json;
    
    if (sqlite3_open("health_data.db", &db) == SQLITE_OK) {
        json << "{\"records\":[";
        
        // Crear tabla si no existe
        const char* create_table = 
            "CREATE TABLE IF NOT EXISTS health_records ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "date TEXT,"
            "weight REAL,"
            "systolic INTEGER,"
            "diastolic INTEGER,"
            "glucose REAL);";
        sqlite3_exec(db, create_table, 0, 0, 0);
        
        // Consultar datos
        sqlite3_stmt *stmt;
        const char* query = "SELECT date, weight, systolic, diastolic, glucose FROM health_records";
        
        if (sqlite3_prepare_v2(db, query, -1, &stmt, 0) == SQLITE_OK) {
            bool first = true;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                if (!first) json << ",";
                first = false;
                
                json << "{"
                     << "\"date\":\"" << sqlite3_column_text(stmt, 0) << "\","
                     << "\"weight\":" << sqlite3_column_double(stmt, 1) << ","
                     << "\"systolic\":" << sqlite3_column_int(stmt, 2) << ","
                     << "\"diastolic\":" << sqlite3_column_int(stmt, 3) << ","
                     << "\"glucose\":" << sqlite3_column_double(stmt, 4)
                     << "}";
            }
            sqlite3_finalize(stmt);
        }
        
        sqlite3_close(db);
        json << "]}";
    }
    
    return json.str();
}

int main() {
    std::cout << "Content-type: application/json\n\n";
    std::cout << getHealthRecords();
    return 0;
}
