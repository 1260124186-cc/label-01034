#ifndef USER_FACTORY_H
#define USER_FACTORY_H

#include "User.h"
#include "Student.h"
#include "Teacher.h"
#include <memory>
#include <string>

// 工厂模式 - 创建不同类型的用户
class UserFactory {
public:
    static std::unique_ptr<User> createUser(Role role, int id,
        const std::string& username, const std::string& password,
        const std::string& name);

    static std::unique_ptr<Student> createStudent(int id,
        const std::string& username, const std::string& password,
        const std::string& name, const std::string& studentNo, int majorId);

    static std::unique_ptr<Teacher> createTeacher(int id,
        const std::string& username, const std::string& password,
        const std::string& name, const std::string& teacherNo, int collegeId);
};

#endif // USER_FACTORY_H
