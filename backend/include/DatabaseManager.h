#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <string>
#include <sqlite3.h>

// 单例模式 - 数据库连接管理器
class DatabaseManager {
private:
    sqlite3* db;
    std::string dbPath;
    static DatabaseManager* instance;

    DatabaseManager();
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    void createTables();

public:
    static DatabaseManager* getInstance();
    static void destroyInstance();

    sqlite3* getDB() const { return db; }
    bool open(const std::string& path);
    void close();
    bool execute(const std::string& sql);
};

#endif // DATABASE_MANAGER_H
