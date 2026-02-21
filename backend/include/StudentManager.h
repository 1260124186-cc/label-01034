#ifndef STUDENT_MANAGER_H
#define STUDENT_MANAGER_H

#include "Student.h"
#include "Course.h"
#include <vector>

class StudentManager {
public:
    // 选课
    bool selectCourse(int studentId, int courseId, std::string& errorMsg);
    // 退选
    bool dropCourse(int studentId, int courseId);
    // 获取学生已选课程
    std::vector<Course> getSelectedCourses(int studentId);
    // 检查是否已选某课程
    bool hasSelected(int studentId, int courseId);
};

#endif // STUDENT_MANAGER_H
