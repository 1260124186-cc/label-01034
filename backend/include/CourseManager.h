#ifndef COURSE_MANAGER_H
#define COURSE_MANAGER_H

#include "Course.h"
#include <vector>
#include <string>

class CourseManager {
public:
    bool addCourse(const std::string& name, int credit, const std::string& semester,
                   int capacity, int teacherId, const std::string& schedule);
    bool updateCourse(int id, const std::string& name, int credit,
                      const std::string& semester, int capacity, const std::string& schedule);
    bool deleteCourse(int id);
    std::vector<Course> getAllCourses();
    std::vector<Course> getCoursesByTeacher(int teacherId);
    Course getCourseById(int id);
    bool incrementEnrolled(int courseId);
    bool decrementEnrolled(int courseId);
};

#endif // COURSE_MANAGER_H
