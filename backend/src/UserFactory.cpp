#include "UserFactory.h"

std::unique_ptr<User> UserFactory::createUser(Role role, int id,
    const std::string& username, const std::string& password,
    const std::string& name) {
    switch (role) {
        case Role::ADMIN: {
            auto user = std::make_unique<User>(id, username, password, name, Role::ADMIN);
            return user;
        }
        case Role::TEACHER: {
            auto teacher = std::make_unique<Teacher>(id, username, password, name, "", 0);
            return teacher;
        }
        case Role::STUDENT: {
            auto student = std::make_unique<Student>(id, username, password, name, "", 0);
            return student;
        }
        default:
            return nullptr;
    }
}

std::unique_ptr<Student> UserFactory::createStudent(int id,
    const std::string& username, const std::string& password,
    const std::string& name, const std::string& studentNo, int majorId) {
    return std::make_unique<Student>(id, username, password, name, studentNo, majorId);
}

std::unique_ptr<Teacher> UserFactory::createTeacher(int id,
    const std::string& username, const std::string& password,
    const std::string& name, const std::string& teacherNo, int collegeId) {
    return std::make_unique<Teacher>(id, username, password, name, teacherNo, collegeId);
}
