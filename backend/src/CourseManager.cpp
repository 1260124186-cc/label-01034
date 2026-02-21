#include "CourseManager.h"
#include "DatabaseManager.h"
#include <sqlite3.h>

bool CourseManager::addCourse(const std::string& name, int credit, const std::string& semester,
                               int capacity, int teacherId, const std::string& schedule) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO courses (name, credit, semester, capacity, enrolled, teacher_id, schedule) VALUES (?, ?, ?, ?, 0, ?, ?);";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, credit);
    sqlite3_bind_text(stmt, 3, semester.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, capacity);
    sqlite3_bind_int(stmt, 5, teacherId);
    sqlite3_bind_text(stmt, 6, schedule.c_str(), -1, SQLITE_TRANSIENT);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

bool CourseManager::updateCourse(int id, const std::string& name, int credit,
                                  const std::string& semester, int capacity, const std::string& schedule) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "UPDATE courses SET name = ?, credit = ?, semester = ?, capacity = ?, schedule = ? WHERE id = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, credit);
    sqlite3_bind_text(stmt, 3, semester.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, capacity);
    sqlite3_bind_text(stmt, 5, schedule.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, id);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

bool CourseManager::deleteCourse(int id) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    // 先删除相关选课记录
    DatabaseManager::getInstance()->execute(
        "DELETE FROM course_selections WHERE course_id = " + std::to_string(id) + ";");
    sqlite3_stmt* stmt;
    std::string sql = "DELETE FROM courses WHERE id = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

std::vector<Course> CourseManager::getAllCourses() {
    std::vector<Course> courses;
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "SELECT id, name, credit, semester, capacity, enrolled, teacher_id, schedule FROM courses;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Course c;
        c.setId(sqlite3_column_int(stmt, 0));
        c.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        c.setCredit(sqlite3_column_int(stmt, 2));
        c.setSemester(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        c.setCapacity(sqlite3_column_int(stmt, 4));
        c.setEnrolled(sqlite3_column_int(stmt, 5));
        c.setTeacherId(sqlite3_column_int(stmt, 6));
        const char* sch = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        c.setSchedule(sch ? sch : "");
        courses.push_back(c);
    }
    sqlite3_finalize(stmt);
    return courses;
}

std::vector<Course> CourseManager::getCoursesByTeacher(int teacherId) {
    std::vector<Course> courses;
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "SELECT id, name, credit, semester, capacity, enrolled, teacher_id, schedule FROM courses WHERE teacher_id = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, teacherId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Course c;
        c.setId(sqlite3_column_int(stmt, 0));
        c.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        c.setCredit(sqlite3_column_int(stmt, 2));
        c.setSemester(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        c.setCapacity(sqlite3_column_int(stmt, 4));
        c.setEnrolled(sqlite3_column_int(stmt, 5));
        c.setTeacherId(sqlite3_column_int(stmt, 6));
        const char* sch = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        c.setSchedule(sch ? sch : "");
        courses.push_back(c);
    }
    sqlite3_finalize(stmt);
    return courses;
}

Course CourseManager::getCourseById(int id) {
    Course c;
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "SELECT id, name, credit, semester, capacity, enrolled, teacher_id, schedule FROM courses WHERE id = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        c.setId(sqlite3_column_int(stmt, 0));
        c.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        c.setCredit(sqlite3_column_int(stmt, 2));
        c.setSemester(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        c.setCapacity(sqlite3_column_int(stmt, 4));
        c.setEnrolled(sqlite3_column_int(stmt, 5));
        c.setTeacherId(sqlite3_column_int(stmt, 6));
        const char* sch = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        c.setSchedule(sch ? sch : "");
    }
    sqlite3_finalize(stmt);
    return c;
}

bool CourseManager::incrementEnrolled(int courseId) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "UPDATE courses SET enrolled = enrolled + 1 WHERE id = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, courseId);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

bool CourseManager::decrementEnrolled(int courseId) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "UPDATE courses SET enrolled = enrolled - 1 WHERE id = ? AND enrolled > 0;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, courseId);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}
