#include "DatabaseManager.h"
#include <iostream>

DatabaseManager* DatabaseManager::instance = nullptr;

DatabaseManager::DatabaseManager() : db(nullptr), dbPath("") {}

DatabaseManager::~DatabaseManager() {
    close();
}

DatabaseManager* DatabaseManager::getInstance() {
    if (instance == nullptr) {
        instance = new DatabaseManager();
    }
    return instance;
}

void DatabaseManager::destroyInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

bool DatabaseManager::open(const std::string& path) {
    dbPath = path;
    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "无法打开数据库: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    // 不启用外键约束，由应用层保证数据一致性
    // 避免注册时因关联表数据不存在而失败
    execute("PRAGMA foreign_keys = OFF;");
    createTables();
    return true;
}

void DatabaseManager::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool DatabaseManager::execute(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL错误: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

void DatabaseManager::createTables() {
    // 用户表
    execute(R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            name TEXT NOT NULL,
            role INTEGER NOT NULL DEFAULT 2
        );
    )");

    // 学生表
    execute(R"(
        CREATE TABLE IF NOT EXISTS students (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            student_no TEXT UNIQUE NOT NULL,
            major_id INTEGER,
            FOREIGN KEY (user_id) REFERENCES users(id),
            FOREIGN KEY (major_id) REFERENCES majors(id)
        );
    )");

    // 教师表
    execute(R"(
        CREATE TABLE IF NOT EXISTS teachers (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            teacher_no TEXT UNIQUE NOT NULL,
            college_id INTEGER,
            FOREIGN KEY (user_id) REFERENCES users(id),
            FOREIGN KEY (college_id) REFERENCES colleges(id)
        );
    )");

    // 学院表
    execute(R"(
        CREATE TABLE IF NOT EXISTS colleges (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT UNIQUE NOT NULL,
            description TEXT
        );
    )");

    // 专业表
    execute(R"(
        CREATE TABLE IF NOT EXISTS majors (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            college_id INTEGER NOT NULL,
            FOREIGN KEY (college_id) REFERENCES colleges(id)
        );
    )");

    // 课程表
    execute(R"(
        CREATE TABLE IF NOT EXISTS courses (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            credit INTEGER NOT NULL,
            semester TEXT NOT NULL,
            capacity INTEGER NOT NULL,
            enrolled INTEGER NOT NULL DEFAULT 0,
            teacher_id INTEGER NOT NULL,
            schedule TEXT,
            FOREIGN KEY (teacher_id) REFERENCES teachers(id)
        );
    )");

    // 选课记录表
    execute(R"(
        CREATE TABLE IF NOT EXISTS course_selections (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            student_id INTEGER NOT NULL,
            course_id INTEGER NOT NULL,
            select_time TEXT DEFAULT (datetime('now','localtime')),
            UNIQUE(student_id, course_id),
            FOREIGN KEY (student_id) REFERENCES students(id),
            FOREIGN KEY (course_id) REFERENCES courses(id)
        );
    )");

    // 插入默认管理员账号（如果不存在）
    execute(R"(
        INSERT OR IGNORE INTO users (username, password, name, role)
        VALUES ('admin', 'admin123', '系统管理员', 0);
    )");
}
