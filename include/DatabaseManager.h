#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QCoreApplication>

class DatabaseManager {
public:
    // Singleton pattern
    static DatabaseManager& instance();
    
    // Inicialización
    bool initialize();
    
    // Pruebas
    bool testConnection();
    QString getDatabaseInfo();
    
    // Operaciones de usuarios
    bool createUser(const QString &username, const QString &password, const QString &email = "");
    bool authenticateUser(const QString &username, const QString &password);
    
    // Operaciones de registros de salud
    bool addHealthRecord(int userId, double weight, int systolic, int diastolic, double glucose, const QString &notes = "");
    QVector<QStringList> getHealthRecords(int userId);
    
private:
    // Constructor y destructor privados
    DatabaseManager();
    ~DatabaseManager();
    
    // Métodos internos
    bool createTables();
    QString hashPassword(const QString &password);
    QString getDatabasePath() const;
    
    // Miembros
    QSqlDatabase m_database;
    bool m_initialized;
    
    // Eliminar copias
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
};

#endif // DATABASEMANAGER_H
