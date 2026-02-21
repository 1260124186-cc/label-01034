#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include <string>

class Student : public User {
private:
    std::string studentNo;  // 学号
    int majorId;            // 所属专业ID

public:
    Student() : User(), majorId(0) { role = Role::STUDENT; }
    Student(int id, const std::string& username, const std::string& password,
            const std::string& name, const std::string& studentNo, int majorId)
        : User(id, username, password, name, Role::STUDENT),
          studentNo(studentNo), majorId(majorId) {}

    std::string getStudentNo() const { return studentNo; }
    int getMajorId() const { return majorId; }

    void setStudentNo(const std::string& sno) { studentNo = sno; }
    void setMajorId(int mid) { majorId = mid; }

    std::string getRoleString() const override { return "学生"; }
};

#endif // STUDENT_H
