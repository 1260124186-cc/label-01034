#ifndef TEACHER_H
#define TEACHER_H

#include "User.h"
#include <string>

class Teacher : public User {
private:
    std::string teacherNo;  // 工号
    int collegeId;          // 所属学院ID

public:
    Teacher() : User(), collegeId(0) { role = Role::TEACHER; }
    Teacher(int id, const std::string& username, const std::string& password,
            const std::string& name, const std::string& teacherNo, int collegeId)
        : User(id, username, password, name, Role::TEACHER),
          teacherNo(teacherNo), collegeId(collegeId) {}

    std::string getTeacherNo() const { return teacherNo; }
    int getCollegeId() const { return collegeId; }

    void setTeacherNo(const std::string& tno) { teacherNo = tno; }
    void setCollegeId(int cid) { collegeId = cid; }

    std::string getRoleString() const override { return "教师"; }
};

#endif // TEACHER_H
