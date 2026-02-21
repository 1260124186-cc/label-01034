#ifndef COLLEGE_MANAGER_H
#define COLLEGE_MANAGER_H

#include "College.h"
#include <vector>
#include <string>

class CollegeManager {
public:
    bool addCollege(const std::string& name, const std::string& description);
    bool updateCollege(int id, const std::string& name, const std::string& description);
    bool deleteCollege(int id);
    std::vector<College> getAllColleges();
    College getCollegeById(int id);
    // 检查学院下是否有关联的专业
    bool hasRelatedMajors(int collegeId);
    // 检查学院下是否有关联的教师
    bool hasRelatedTeachers(int collegeId);
};

#endif // COLLEGE_MANAGER_H
