#ifndef TEACHER_MANAGER_H
#define TEACHER_MANAGER_H

#include "Teacher.h"
#include "Student.h"
#include <vector>

class TeacherManager {
public:
    // 获取教师所授课程的学生名单
    std::vector<Student> getStudentsByCourse(int courseId);
};

#endif // TEACHER_MANAGER_H
