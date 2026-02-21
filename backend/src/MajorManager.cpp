#include "MajorManager.h"
#include "DatabaseManager.h"
#include <sqlite3.h>

bool MajorManager::addMajor(const std::string& name, int collegeId) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO majors (name, college_id) VALUES (?, ?);";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, collegeId);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

bool MajorManager::updateMajor(int id, const std::string& name, int collegeId) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "UPDATE majors SET name = ?, college_id = ? WHERE id = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, collegeId);
    sqlite3_bind_int(stmt, 3, id);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

bool MajorManager::deleteMajor(int id) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "DELETE FROM majors WHERE id = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

std::vector<Major> MajorManager::getAllMajors() {
    std::vector<Major> majors;
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "SELECT id, name, college_id FROM majors;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Major m;
        m.setId(sqlite3_column_int(stmt, 0));
        m.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        m.setCollegeId(sqlite3_column_int(stmt, 2));
        majors.push_back(m);
    }
    sqlite3_finalize(stmt);
    return majors;
}

std::vector<Major> MajorManager::getMajorsByCollege(int collegeId) {
    std::vector<Major> majors;
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "SELECT id, name, college_id FROM majors WHERE college_id = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, collegeId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Major m;
        m.setId(sqlite3_column_int(stmt, 0));
        m.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        m.setCollegeId(sqlite3_column_int(stmt, 2));
        majors.push_back(m);
    }
    sqlite3_finalize(stmt);
    return majors;
}

Major MajorManager::getMajorById(int id) {
    Major m;
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "SELECT id, name, college_id FROM majors WHERE id = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        m.setId(sqlite3_column_int(stmt, 0));
        m.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        m.setCollegeId(sqlite3_column_int(stmt, 2));
    }
    sqlite3_finalize(stmt);
    return m;
}

bool MajorManager::hasRelatedStudents(int majorId) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "SELECT COUNT(*) FROM students WHERE major_id = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, majorId);
    bool has = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        has = sqlite3_column_int(stmt, 0) > 0;
    }
    sqlite3_finalize(stmt);
    return has;
}
