#ifndef MAJOR_MANAGER_H
#define MAJOR_MANAGER_H

#include "Major.h"
#include <vector>
#include <string>

class MajorManager {
public:
    bool addMajor(const std::string& name, int collegeId);
    bool updateMajor(int id, const std::string& name, int collegeId);
    bool deleteMajor(int id);
    std::vector<Major> getAllMajors();
    std::vector<Major> getMajorsByCollege(int collegeId);
    Major getMajorById(int id);
};

#endif // MAJOR_MANAGER_H
