#ifndef SELECTION_STRATEGY_H
#define SELECTION_STRATEGY_H

#include "Course.h"
#include <string>
#include <vector>
#include <set>

// 策略模式 - 选课策略接口
class SelectionStrategy {
public:
    virtual ~SelectionStrategy() = default;
    // 检查是否允许选课，返回true表示允许，errorMsg返回错误信息
    virtual bool canSelect(int studentId, const Course& course, std::string& errorMsg) = 0;
};

// 先到先得策略（检查容量）
class FirstComeStrategy : public SelectionStrategy {
public:
    bool canSelect(int studentId, const Course& course, std::string& errorMsg) override;
};

// 时间冲突检测策略
class TimeConflictStrategy : public SelectionStrategy {
public:
    bool canSelect(int studentId, const Course& course, std::string& errorMsg) override;

private:
    // 解析时间表字符串为时间段集合
    std::set<std::string> parseSchedule(const std::string& schedule);
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
