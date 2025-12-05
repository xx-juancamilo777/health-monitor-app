#include "DatabaseManager.h"
#include <QCryptographicHash>

DatabaseManager::DatabaseManager() 
    : m_initialized(false) {
    // Constructor privado
}

DatabaseManager::~DatabaseManager() {
    if (m_database.isOpen()) {
        m_database.close();
        qDebug() << "✅ Base de datos cerrada correctamente";
    }
}

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

QString DatabaseManager::getDatabasePath() const {
    // Base de datos en la carpeta del ejecutable
    QString appDir = QCoreApplication::applicationDirPath();
    return appDir + "/health_data.db";
}

bool DatabaseManager::initialize() {
    if (m_initialized) {
        return true;
    }
    
    // Configurar conexión SQLite
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = getDatabasePath();
    m_database.setDatabaseName(dbPath);
    
    qDebug() << "🔧 Inicializando base de datos...";
    qDebug() << "📁 Ruta:" << dbPath;
    
    // Intentar abrir la base de datos
    if (!m_database.open()) {
        QSqlError error = m_database.lastError();
        qDebug() << "❌ Error al abrir base de datos:" << error.text();
        
        QMessageBox::critical(nullptr, "Error de Base de Datos",
            QString("No se pudo abrir la base de datos:\n\n"
                   "Error: %1\n"
                   "Ruta: %2\n\n"
                   "Verifica permisos de escritura.")
            .arg(error.text())
            .arg(dbPath));
        
        return false;
    }
    
    qDebug() << "✅ Base de datos abierta correctamente";
    
    // Crear tablas
    if (!createTables()) {
        qDebug() << "❌ Error al crear tablas";
        return false;
    }
    
    m_initialized = true;
    qDebug() << "✅ Sistema de base de datos inicializado completamente";
    
    return true;
}

bool DatabaseManager::createTables() {
    QSqlQuery query;
    
    // Tabla de usuarios
    QString usersTable = 
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE NOT NULL,"
        "password_hash TEXT NOT NULL,"
        "email TEXT,"
        "full_name TEXT,"
        "birth_date DATE,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "updated_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";
    
    if (!query.exec(usersTable)) {
        qDebug() << "❌ Error al crear tabla users:" << query.lastError().text();
        return false;
    }
    
    qDebug() << "✅ Tabla 'users' creada/verificada";
    
    // Tabla de registros de salud
    QString healthRecordsTable = 
        "CREATE TABLE IF NOT EXISTS health_records ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "user_id INTEGER NOT NULL,"
        "record_date DATE NOT NULL,"
        "record_time TIME NOT NULL,"
        "weight REAL CHECK(weight > 0 AND weight < 300),"
        "systolic INTEGER CHECK(systolic > 50 AND systolic < 250),"
        "diastolic INTEGER CHECK(diastolic > 30 AND diastolic < 150),"
        "glucose REAL CHECK(glucose >= 0 AND glucose < 1000),"
        "notes TEXT,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE"
        ");";
    
    if (!query.exec(healthRecordsTable)) {
        qDebug() << "❌ Error al crear tabla health_records:" << query.lastError().text();
        return false;
    }
    
    qDebug() << "✅ Tabla 'health_records' creada/verificada";
    
    // Índices para mejor rendimiento
    query.exec("CREATE INDEX IF NOT EXISTS idx_user_id ON health_records(user_id)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_record_date ON health_records(record_date)");
    
    qDebug() << "✅ Índices de base de datos creados/verificados";
    
    return true;
}

QString DatabaseManager::hashPassword(const QString &password) {
    // Hash seguro con SHA-256
    QByteArray hash = QCryptographicHash::hash(
        password.toUtf8(),
        QCryptographicHash::Sha256
    );
    return QString(hash.toHex());
}

bool DatabaseManager::testConnection() {
    if (!m_initialized) {
        return false;
    }
    
    QSqlQuery query("SELECT 1");
    return query.exec() && query.next();
}

QString DatabaseManager::getDatabaseInfo() {
    if (!m_initialized) {
        return "Base de datos no inicializada";
    }
    
    QString info;
    info += "🔧 Estado: CONECTADO\n";
    info += "📁 Driver: " + m_database.driverName() + "\n";
    info += "🗃️  Base: " + m_database.databaseName() + "\n";
    
    // Contar registros
    QSqlQuery query;
    
    query.exec("SELECT COUNT(*) FROM users");
    if (query.next()) {
        info += "👥 Usuarios: " + QString::number(query.value(0).toInt()) + "\n";
    }
    
    query.exec("SELECT COUNT(*) FROM health_records");
    if (query.next()) {
        info += "📊 Registros: " + QString::number(query.value(0).toInt()) + "\n";
    }
    
    return info;
}

bool DatabaseManager::createUser(const QString &username, const QString &password, const QString &email) {
    if (!m_initialized) {
        qDebug() << "❌ Base de datos no inicializada";
        return false;
    }
    
    QString hashedPassword = hashPassword(password);
    
    QSqlQuery query;
    query.prepare(
        "INSERT INTO users (username, password_hash, email) "
        "VALUES (:username, :password, :email)"
    );
    
    query.bindValue(":username", username);
    query.bindValue(":password", hashedPassword);
    query.bindValue(":email", email);
    
    if (!query.exec()) {
        qDebug() << "❌ Error al crear usuario:" << query.lastError().text();
        return false;
    }
    
    qDebug() << "✅ Usuario creado:" << username;
    return true;
}

bool DatabaseManager::authenticateUser(const QString &username, const QString &password) {
    if (!m_initialized) {
        return false;
    }
    
    QString hashedPassword = hashPassword(password);
    
    QSqlQuery query;
    query.prepare(
        "SELECT id FROM users WHERE username = :username AND password_hash = :password"
    );
    
    query.bindValue(":username", username);
    query.bindValue(":password", hashedPassword);
    
    if (!query.exec()) {
        qDebug() << "❌ Error en autenticación:" << query.lastError().text();
        return false;
    }
    
    return query.next(); // Si hay resultados, usuario válido
}

bool DatabaseManager::addHealthRecord(int userId, double weight, int systolic, 
                                     int diastolic, double glucose, const QString &notes) {
    if (!m_initialized) {
        return false;
    }
    
    QSqlQuery query;
    query.prepare(
        "INSERT INTO health_records "
        "(user_id, record_date, record_time, weight, systolic, diastolic, glucose, notes) "
        "VALUES (:user_id, DATE('now'), TIME('now'), :weight, :systolic, :diastolic, :glucose, :notes)"
    );
    
    query.bindValue(":user_id", userId);
    query.bindValue(":weight", weight);
    query.bindValue(":systolic", systolic);
    query.bindValue(":diastolic", diastolic);
    query.bindValue(":glucose", glucose);
    query.bindValue(":notes", notes);
    
    if (!query.exec()) {
        qDebug() << "❌ Error al añadir registro:" << query.lastError().text();
        return false;
    }
    
    qDebug() << "✅ Registro de salud añadido para usuario ID:" << userId;
    return true;
}

QVector<QStringList> DatabaseManager::getHealthRecords(int userId) {
    QVector<QStringList> records;
    
    if (!m_initialized) {
        return records;
    }
    
    QSqlQuery query;
    query.prepare(
        "SELECT record_date, record_time, weight, systolic, diastolic, glucose, notes "
        "FROM health_records "
        "WHERE user_id = :user_id "
        "ORDER BY record_date DESC, record_time DESC"
    );
    
    query.bindValue(":user_id", userId);
    
    if (!query.exec()) {
        qDebug() << "❌ Error al obtener registros:" << query.lastError().text();
        return records;
    }
    
    while (query.next()) {
        QStringList record;
        record << query.value(0).toString(); // fecha
        record << query.value(1).toString(); // hora
        record << query.value(2).toString(); // peso
        record << query.value(3).toString(); // sistólica
        record << query.value(4).toString(); // diastólica
        record << query.value(5).toString(); // glucosa
        record << query.value(6).toString(); // notas
        
        records.append(record);
    }
    
    qDebug() << "✅ Obtenidos" << records.size() << "registros para usuario ID:" << userId;
    return records;
}
