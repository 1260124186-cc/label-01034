#include "UserManager.h"
#include "DatabaseManager.h"
#include <sqlite3.h>
#include <iostream>
#include <algorithm>

// 去除字符串首尾空白
static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

bool UserManager::usernameExists(const std::string& username) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string trimmed = trim(username);
    std::string sql = "SELECT COUNT(*) FROM users WHERE username = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, trimmed.c_str(), -1, SQLITE_TRANSIENT);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0) > 0;
    }
    sqlite3_finalize(stmt);
    return exists;
}

bool UserManager::registerAdmin(const std::string& username, const std::string& password, const std::string& name) {
    if (usernameExists(username)) return false;
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO users (username, password, name, role) VALUES (?, ?, ?, 0);";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, name.c_str(), -1, SQLITE_TRANSIENT);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

bool UserManager::registerStudent(const std::string& username, const std::string& password,
                                   const std::string& name, const std::string& studentNo, int majorId) {
    if (usernameExists(username)) return false;
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    // 使用事务保证原子性
    DatabaseManager::getInstance()->execute("BEGIN TRANSACTION;");

    // 插入用户表
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO users (username, password, name, role) VALUES (?, ?, ?, 2);";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, name.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        DatabaseManager::getInstance()->execute("ROLLBACK;");
        return false;
    }
    sqlite3_finalize(stmt);

    int userId = (int)sqlite3_last_insert_rowid(db);

    // 插入学生表
    sql = "INSERT INTO students (user_id, student_no, major_id) VALUES (?, ?, ?);";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_text(stmt, 2, studentNo.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, majorId);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        DatabaseManager::getInstance()->execute("ROLLBACK;");
        return false;
    }
    sqlite3_finalize(stmt);

    DatabaseManager::getInstance()->execute("COMMIT;");
    return true;
}

bool UserManager::registerTeacher(const std::string& username, const std::string& password,
                                   const std::string& name, const std::string& teacherNo, int collegeId) {
    if (usernameExists(username)) return false;
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    // 使用事务保证原子性
    DatabaseManager::getInstance()->execute("BEGIN TRANSACTION;");

    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO users (username, password, name, role) VALUES (?, ?, ?, 1);";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, name.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        DatabaseManager::getInstance()->execute("ROLLBACK;");
        return false;
    }
    sqlite3_finalize(stmt);

    int userId = (int)sqlite3_last_insert_rowid(db);

    sql = "INSERT INTO teachers (user_id, teacher_no, college_id) VALUES (?, ?, ?);";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_text(stmt, 2, teacherNo.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, collegeId);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        DatabaseManager::getInstance()->execute("ROLLBACK;");
        return false;
    }
    sqlite3_finalize(stmt);

    DatabaseManager::getInstance()->execute("COMMIT;");
    return true;
}

int UserManager::login(const std::string& username, const std::string& password, Role& outRole) {
    std::string trimUser = trim(username);
    std::string trimPass = trim(password);

    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "SELECT id, role FROM users WHERE username = ? AND password = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, trimUser.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, trimPass.c_str(), -1, SQLITE_TRANSIENT);

    int userId = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        userId = sqlite3_column_int(stmt, 0);
        int roleInt = sqlite3_column_int(stmt, 1);
        outRole = static_cast<Role>(roleInt);
    }
    sqlite3_finalize(stmt);
    return userId;
}

std::vector<User> UserManager::getAllUsers() {
    std::vector<User> users;
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "SELECT id, username, password, name, role FROM users;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        User u;
        u.setId(sqlite3_column_int(stmt, 0));
        u.setUsername(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        u.setPassword(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        u.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        u.setRole(static_cast<Role>(sqlite3_column_int(stmt, 4)));
        users.push_back(u);
    }
    sqlite3_finalize(stmt);
    return users;
}

Student UserManager::getStudentByUserId(int userId) {
    Student s;
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = R"(
        SELECT u.id, u.username, u.password, u.name, s.student_no, s.major_id, s.id
        FROM users u JOIN students s ON u.id = s.user_id
        WHERE u.id = ?;
    )";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, userId);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        s.setId(sqlite3_column_int(stmt, 6)); // student table id
        s.setUsername(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        s.setPassword(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        s.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        s.setStudentNo(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        s.setMajorId(sqlite3_column_int(stmt, 5));
        s.setRole(Role::STUDENT);
    }
    sqlite3_finalize(stmt);
    return s;
}

Teacher UserManager::getTeacherByUserId(int userId) {
    Teacher t;
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = R"(
        SELECT u.id, u.username, u.password, u.name, t.teacher_no, t.college_id, t.id
        FROM users u JOIN teachers t ON u.id = t.user_id
        WHERE u.id = ?;
    )";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, userId);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        t.setId(sqlite3_column_int(stmt, 6)); // teacher table id
        t.setUsername(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        t.setPassword(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        t.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        t.setTeacherNo(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        t.setCollegeId(sqlite3_column_int(stmt, 5));
        t.setRole(Role::TEACHER);
    }
    sqlite3_finalize(stmt);
    return t;
}
