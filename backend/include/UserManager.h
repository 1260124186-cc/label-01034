#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "User.h"
#include "Student.h"
#include "Teacher.h"
#include <vector>
#include <memory>
#include <string>

class UserManager {
public:
    // 用户注册
    bool registerAdmin(const std::string& username, const std::string& password, const std::string& name);
    bool registerStudent(const std::string& username, const std::string& password,
                         const std::string& name, const std::string& studentNo, int majorId);
    bool registerTeacher(const std::string& username, const std::string& password,
                         const std::string& name, const std::string& teacherNo, int collegeId);

    // 用户登录，返回用户ID和角色；失败返回-1
    int login(const std::string& username, const std::string& password, Role& outRole);

    // 查询
    std::vector<User> getAllUsers();
    bool usernameExists(const std::string& username);
    User getUserById(int userId);

    // 获取学生/教师详细信息
    Student getStudentByUserId(int userId);
    Teacher getTeacherByUserId(int userId);

    // 管理员用户管理
    bool updateUserName(int userId, const std::string& newName);
    bool resetPassword(int userId, const std::string& newPassword);
    bool deleteUser(int userId);
};

#endif // USER_MANAGER_H
