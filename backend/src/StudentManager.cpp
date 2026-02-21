#include "StudentManager.h"
#include "CourseManager.h"
#include "SelectionStrategy.h"
#include "DatabaseManager.h"
#include "Logger.h"
#include <sqlite3.h>

bool StudentManager::hasSelected(int studentId, int courseId) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "SELECT COUNT(*) FROM course_selections WHERE student_id = ? AND course_id = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, studentId);
    sqlite3_bind_int(stmt, 2, courseId);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0) > 0;
    }
    sqlite3_finalize(stmt);
    return exists;
}

bool StudentManager::selectCourse(int studentId, int courseId, std::string& errorMsg) {
    // 检查是否已选
    if (hasSelected(studentId, courseId)) {
        errorMsg = "您已选过该课程，请勿重复选课";
        return false;
    }

    // 获取课程信息
    CourseManager cm;
    Course course = cm.getCourseById(courseId);
    if (course.getId() == 0) {
        errorMsg = "课程不存在";
        return false;
    }

    // 使用组合策略检查
    FirstComeStrategy firstCome;
    TimeConflictStrategy timeConflict;
    CompositeStrategy composite;
    composite.addStrategy(&firstCome);
    composite.addStrategy(&timeConflict);

    if (!composite.canSelect(studentId, course, errorMsg)) {
        return false;
    }

    // 插入选课记录
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO course_selections (student_id, course_id) VALUES (?, ?);";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, studentId);
    sqlite3_bind_int(stmt, 2, courseId);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        errorMsg = "选课失败，数据库错误";
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);

    // 更新课程已选人数
    cm.incrementEnrolled(courseId);
    Logger::getInstance()->info("选课成功: 学生ID=" + std::to_string(studentId) + " 课程ID=" + std::to_string(courseId), "StudentManager");
    return true;
}

bool StudentManager::dropCourse(int studentId, int courseId) {
    if (!hasSelected(studentId, courseId)) return false;

    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = "DELETE FROM course_selections WHERE student_id = ? AND course_id = ?;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, studentId);
    sqlite3_bind_int(stmt, 2, courseId);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);

    if (ok) {
        CourseManager cm;
        cm.decrementEnrolled(courseId);
        Logger::getInstance()->info("退选成功: 学生ID=" + std::to_string(studentId) + " 课程ID=" + std::to_string(courseId), "StudentManager");
    }
    return ok;
}

std::vector<Course> StudentManager::getSelectedCourses(int studentId) {
    std::vector<Course> courses;
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = R"(
        SELECT c.id, c.name, c.credit, c.semester, c.capacity, c.enrolled, c.teacher_id, c.schedule
        FROM courses c
        JOIN course_selections cs ON c.id = cs.course_id
        WHERE cs.student_id = ?;
    )";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, studentId);

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
