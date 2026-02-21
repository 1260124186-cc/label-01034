#include "SelectionStrategy.h"
#include "DatabaseManager.h"
#include "CourseManager.h"
#include "Logger.h"
#include <sqlite3.h>
#include <sstream>
#include <regex>

// ========== 先到先得策略 ==========
bool FirstComeStrategy::canSelect(int studentId, const Course& course, std::string& errorMsg) {
    if (course.isFull()) {
        errorMsg = "课程已满，无法选课";
        return false;
    }
    return true;
}

// ========== 时间冲突检测策略 ==========

std::vector<TimeSlot> TimeConflictStrategy::parseSchedule(const std::string& schedule) {
    std::vector<TimeSlot> slots;
    if (schedule.empty()) return slots;

    // 合法的星期前缀（UTF-8）
    static const std::vector<std::string> validDays = {
        "周一", "周二", "周三", "周四", "周五", "周六", "周日"
    };

    // 按逗号分割
    std::stringstream ss(schedule);
    std::string token;
    while (std::getline(ss, token, ',')) {
        // 去除首尾空格
        size_t start = token.find_first_not_of(' ');
        size_t end = token.find_last_not_of(' ');
        if (start == std::string::npos) continue;
        token = token.substr(start, end - start + 1);

        // 匹配星期前缀
        for (const auto& day : validDays) {
            if (token.size() > day.size() && token.substr(0, day.size()) == day) {
                std::string remaining = token.substr(day.size());

                // 解析 "数字-数字" 格式
                std::regex timePattern(R"((\d{1,2})-(\d{1,2}))");
                std::smatch match;
                if (std::regex_match(remaining, match, timePattern)) {
                    TimeSlot slot;
                    slot.day = day;
                    slot.startPeriod = std::stoi(match[1].str());
                    slot.endPeriod = std::stoi(match[2].str());
                    // 确保 start <= end
                    if (slot.startPeriod > slot.endPeriod) {
                        std::swap(slot.startPeriod, slot.endPeriod);
                    }
                    slots.push_back(slot);
                }
                break;
            }
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
    std::vector<TimeSlot> newSlots = parseSchedule(course.getSchedule());
    if (newSlots.empty()) return true; // 没有时间信息则不检测

    std::vector<std::string> existingSchedules = getStudentSchedules(studentId);
    for (const auto& sch : existingSchedules) {
        std::vector<TimeSlot> existingSlots = parseSchedule(sch);
        for (const auto& newSlot : newSlots) {
            for (const auto& existSlot : existingSlots) {
                if (newSlot.overlapsWith(existSlot)) {
                    // 构造冲突描述
                    std::string conflictDesc = newSlot.day +
                        std::to_string(newSlot.startPeriod) + "-" + std::to_string(newSlot.endPeriod) +
                        " 与已选课程 " + existSlot.day +
                        std::to_string(existSlot.startPeriod) + "-" + std::to_string(existSlot.endPeriod);
                    errorMsg = "时间冲突：" + conflictDesc;
                    Logger::getInstance()->warn("选课时间冲突: 学生ID=" + std::to_string(studentId) + " " + conflictDesc, "TimeConflictStrategy");
                    return false;
                }
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
