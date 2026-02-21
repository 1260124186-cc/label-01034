#ifndef SELECTION_STRATEGY_H
#define SELECTION_STRATEGY_H

#include "Course.h"
#include <string>
#include <vector>
#include <set>

// 时间段结构体：星期 + 节次区间
struct TimeSlot {
    std::string day;  // "周一"~"周日"
    int startPeriod;  // 起始节次
    int endPeriod;    // 结束节次

    // 判断两个时间段是否重叠
    bool overlapsWith(const TimeSlot& other) const {
        return day == other.day && startPeriod <= other.endPeriod && endPeriod >= other.startPeriod;
    }
};

// 策略模式 - 选课策略接口
class SelectionStrategy {
public:
    virtual ~SelectionStrategy() = default;
    virtual bool canSelect(int studentId, const Course& course, std::string& errorMsg) = 0;
};

// 先到先得策略（检查容量）
class FirstComeStrategy : public SelectionStrategy {
public:
    bool canSelect(int studentId, const Course& course, std::string& errorMsg) override;
};

// 时间冲突检测策略（区间重叠判断）
class TimeConflictStrategy : public SelectionStrategy {
public:
    bool canSelect(int studentId, const Course& course, std::string& errorMsg) override;

private:
    // 解析时间表字符串为 TimeSlot 列表
    std::vector<TimeSlot> parseSchedule(const std::string& schedule);
    // 获取学生已选课程的时间表
    std::vector<std::string> getStudentSchedules(int studentId);
};

// 组合策略（同时检查多个条件）
class CompositeStrategy : public SelectionStrategy {
private:
    std::vector<SelectionStrategy*> strategies;

public:
    void addStrategy(SelectionStrategy* strategy);
    bool canSelect(int studentId, const Course& course, std::string& errorMsg) override;
};

#endif // SELECTION_STRATEGY_H
