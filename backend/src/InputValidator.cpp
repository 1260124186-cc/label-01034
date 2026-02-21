#include "InputValidator.h"
#include <regex>
#include <set>

bool InputValidator::isPositiveInt(int value) {
    return value > 0;
}

bool InputValidator::isCreditValid(int credit) {
    return credit >= 1 && credit <= 10;
}

bool InputValidator::isCapacityValid(int capacity) {
    return capacity >= 1 && capacity <= 500;
}

bool InputValidator::isStudentNoValid(const std::string& sno) {
    if (sno.size() < 6 || sno.size() > 20) return false;
    for (char c : sno) {
        if (!std::isalnum(c)) return false;
    }
    return true;
}

bool InputValidator::isTeacherNoValid(const std::string& tno) {
    if (tno.size() < 3 || tno.size() > 20) return false;
    for (char c : tno) {
        if (!std::isalnum(c)) return false;
    }
    return true;
}

bool InputValidator::isUsernameValid(const std::string& username) {
    if (username.size() < 3 || username.size() > 20) return false;
    for (char c : username) {
        if (!std::isalnum(c) && c != '_') return false;
    }
    return true;
}

bool InputValidator::isPasswordValid(const std::string& password) {
    return password.size() >= 6;
}

bool InputValidator::isNameValid(const std::string& name) {
    return !name.empty() && name.size() <= 50;
}

bool InputValidator::isScheduleValid(const std::string& schedule) {
    if (schedule.empty()) return true; // 允许为空

    // 合法的星期前缀（UTF-8中文）
    static const std::set<std::string> validDays = {
        "周一", "周二", "周三", "周四", "周五", "周六", "周日"
    };

    // 按逗号分割
    std::string s = schedule;
    size_t pos = 0;
    while (pos < s.size()) {
        size_t comma = s.find(',', pos);
        if (comma == std::string::npos) comma = s.size();

        std::string slot = s.substr(pos, comma - pos);
        // 去除首尾空格
        size_t start = slot.find_first_not_of(' ');
        size_t end = slot.find_last_not_of(' ');
        if (start == std::string::npos) return false;
        slot = slot.substr(start, end - start + 1);

        // 检查是否以合法星期开头（UTF-8中文"周X"占6字节）
        bool dayFound = false;
        std::string remaining;
        for (const auto& day : validDays) {
            if (slot.size() > day.size() && slot.substr(0, day.size()) == day) {
                remaining = slot.substr(day.size());
                dayFound = true;
                break;
            }
        }
        if (!dayFound) return false;

        // 检查剩余部分是否为 "数字-数字" 格式
        std::regex timePattern(R"(\d{1,2}-\d{1,2})");
        if (!std::regex_match(remaining, timePattern)) return false;

        pos = comma + 1;
    }
    return true;
}

std::string InputValidator::getStudentNoRule() {
    return "学号要求：6-20位字母或数字";
}

std::string InputValidator::getTeacherNoRule() {
    return "工号要求：3-20位字母或数字";
}

std::string InputValidator::getUsernameRule() {
    return "用户名要求：3-20位字母、数字或下划线";
}

std::string InputValidator::getPasswordRule() {
    return "密码要求：至少6位";
}

std::string InputValidator::getScheduleRule() {
    return "时间格式：周一1-2,周三3-4（多个时间段用逗号分隔）";
}
