#ifndef INPUT_VALIDATOR_H
#define INPUT_VALIDATOR_H

#include <string>

class InputValidator {
public:
    // 数值校验
    static bool isPositiveInt(int value);
    static bool isCreditValid(int credit);       // 学分 1-10
    static bool isCapacityValid(int capacity);    // 容量 1-500

    // 字符串格式校验
    static bool isStudentNoValid(const std::string& sno);   // 学号：字母数字，6-20位
    static bool isTeacherNoValid(const std::string& tno);   // 工号：字母数字，3-20位
    static bool isUsernameValid(const std::string& username); // 用户名：字母数字下划线，3-20位
    static bool isPasswordValid(const std::string& password); // 密码：至少6位
    static bool isNameValid(const std::string& name);         // 姓名：非空，1-50位

    // 时间格式校验 "周X1-2,周X3-4"
    static bool isScheduleValid(const std::string& schedule);

    // 获取校验失败的提示信息
    static std::string getStudentNoRule();
    static std::string getTeacherNoRule();
    static std::string getUsernameRule();
    static std::string getPasswordRule();
    static std::string getScheduleRule();
};

#endif // INPUT_VALIDATOR_H
