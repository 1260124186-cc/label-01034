#include "ReportExporter.h"
#include "StudentManager.h"
#include "TeacherManager.h"
#include "DatabaseManager.h"
#include <fstream>
#include <iostream>
#include <sqlite3.h>

bool ReportExporter::exportCoursesToCSV(const std::vector<Course>& courses, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法创建文件: " << filename << std::endl;
        return false;
    }

    file << "课程ID,课程名称,学分,学期,容量,已选人数,教师ID,上课时间" << std::endl;
    for (const auto& c : courses) {
        file << c.getId() << ","
             << c.getName() << ","
             << c.getCredit() << ","
             << c.getSemester() << ","
             << c.getCapacity() << ","
             << c.getEnrolled() << ","
             << c.getTeacherId() << ","
             << c.getSchedule() << std::endl;
    }

    file.close();
    return true;
}

bool ReportExporter::exportCoursesToTXT(const std::vector<Course>& courses, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法创建文件: " << filename << std::endl;
        return false;
    }

    file << "===== 课程列表 =====" << std::endl;
    file << "------------------------------------------------------" << std::endl;
    for (const auto& c : courses) {
        file << "课程ID: " << c.getId() << std::endl;
        file << "课程名称: " << c.getName() << std::endl;
        file << "学分: " << c.getCredit() << std::endl;
        file << "学期: " << c.getSemester() << std::endl;
        file << "容量: " << c.getCapacity() << "  已选: " << c.getEnrolled() << std::endl;
        file << "上课时间: " << c.getSchedule() << std::endl;
        file << "------------------------------------------------------" << std::endl;
    }

    file.close();
    return true;
}

bool ReportExporter::exportSelectionsToCSV(int studentId, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    StudentManager sm;
    auto courses = sm.getSelectedCourses(studentId);

    file << "课程ID,课程名称,学分,学期,上课时间" << std::endl;
    for (const auto& c : courses) {
        file << c.getId() << ","
             << c.getName() << ","
             << c.getCredit() << ","
             << c.getSemester() << ","
             << c.getSchedule() << std::endl;
    }

    file.close();
    return true;
}

bool ReportExporter::exportCourseStudentsToCSV(int courseId, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    TeacherManager tm;
    auto students = tm.getStudentsByCourse(courseId);

    file << "学生ID,学号,姓名,专业ID" << std::endl;
    for (const auto& s : students) {
        file << s.getId() << ","
             << s.getStudentNo() << ","
             << s.getName() << ","
             << s.getMajorId() << std::endl;
    }

    file.close();
    return true;
}
