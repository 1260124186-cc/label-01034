#ifndef REPORT_EXPORTER_H
#define REPORT_EXPORTER_H

#include "Course.h"
#include "Student.h"
#include <vector>
#include <string>

class ReportExporter {
public:
    // 导出课程列表为CSV
    static bool exportCoursesToCSV(const std::vector<Course>& courses, const std::string& filename);
    // 导出课程列表为TXT
    static bool exportCoursesToTXT(const std::vector<Course>& courses, const std::string& filename);
    // 导出学生选课记录为CSV
    static bool exportSelectionsToCSV(int studentId, const std::string& filename);
    // 导出某课程的学生名单为CSV
    static bool exportCourseStudentsToCSV(int courseId, const std::string& filename);
};

#endif // REPORT_EXPORTER_H
