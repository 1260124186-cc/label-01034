#include "CollegeManager.h"
#include "DatabaseManager.h"
#include <sqlite3.h>

bool CollegeManager::addCollege(const std::string& name, const std::string& description) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO colleges (name, description) VALUES (?, ?);";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, description.c_str(), -1, SQLITE_TRANSIENT);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

bool CollegeManager::updateCollege(int id, const std::string& name, const std::string& description) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "UPDATE colleges SET name = ?, description = ? WHERE id = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, id);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

bool CollegeManager::deleteCollege(int id) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "DELETE FROM colleges WHERE id = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

std::vector<College> CollegeManager::getAllColleges() {
    std::vector<College> colleges;
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "SELECT id, name, description FROM colleges;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        College c;
        c.setId(sqlite3_column_int(stmt, 0));
        c.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        const char* desc = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        c.setDescription(desc ? desc : "");
        colleges.push_back(c);
    }
    sqlite3_finalize(stmt);
    return colleges;
}

College CollegeManager::getCollegeById(int id) {
    College c;
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "SELECT id, name, description FROM colleges WHERE id = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        c.setId(sqlite3_column_int(stmt, 0));
        c.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        const char* desc = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        c.setDescription(desc ? desc : "");
    }
    sqlite3_finalize(stmt);
    return c;
}
