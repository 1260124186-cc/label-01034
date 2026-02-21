#include "SelectionStrategy.h"
#include "DatabaseManager.h"
#include "CourseManager.h"
#include <sqlite3.h>
#include <sstream>

// ========== 先到先得策略 ==========
bool FirstComeStrategy::canSelect(int studentId, const Course& course, std::string& errorMsg) {
    if (course.isFull()) {
        errorMsg = "课程已满，无法选课";
        return false;
    }
    return true;
}

// ========== 时间冲突检测策略 ==========
std::set<std::string> TimeConflictStrategy::parseSchedule(const std::string& schedule) {
    std::set<std::string> slots;
    if (schedule.empty()) return slots;

    std::stringstream ss(schedule);
    std::string token;
    while (std::getline(ss, token, ',')) {
        // 去除首尾空格
        size_t start = token.find_first_not_of(' ');
        size_t end = token.find_last_not_of(' ');
        if (start != std::string::npos) {
            slots.insert(token.substr(start, end - start + 1));
        }
    }
    return slots;
}

std::vector<std::string> TimeConflictStrategy::getStudentSchedules(int studentId) {
    std::vector<std::string> schedules;
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = R"(
        SELECT c.schedule FROM courses c
        JOIN course_selections cs ON c.id = cs.course_id
        WHERE cs.student_id = ?;
    )";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, studentId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* sch = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if (sch) schedules.push_back(sch);
    }
    sqlite3_finalize(stmt);
    return schedules;
}

bool TimeConflictStrategy::canSelect(int studentId, const Course& course, std::string& errorMsg) {
    std::set<std::string> newSlots = parseSchedule(course.getSchedule());
    if (newSlots.empty()) return true; // 没有时间信息则不检测

    std::vector<std::string> existingSchedules = getStudentSchedules(studentId);
    for (const auto& sch : existingSchedules) {
        std::set<std::string> existingSlots = parseSchedule(sch);
        for (const auto& slot : newSlots) {
            if (existingSlots.count(slot)) {
                errorMsg = "时间冲突：与已选课程在 [" + slot + "] 时间段冲突";
                return false;
            }
        }
    }
    return true;
}

// ========== 组合策略 ==========
void CompositeStrategy::addStrategy(SelectionStrategy* strategy) {
    strategies.push_back(strategy);
}

bool CompositeStrategy::canSelect(int studentId, const Course& course, std::string& errorMsg) {
    for (auto* strategy : strategies) {
        if (!strategy->canSelect(studentId, course, errorMsg)) {
            return false;
        }
    }
    return true;
}
