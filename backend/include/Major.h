#ifndef MAJOR_H
#define MAJOR_H

#include <string>

class Major {
private:
    int id;
    std::string name;
    int collegeId;  // 所属学院ID

public:
    Major() : id(0), collegeId(0) {}
    Major(int id, const std::string& name, int collegeId)
        : id(id), name(name), collegeId(collegeId) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    int getCollegeId() const { return collegeId; }

    void setId(int id) { this->id = id; }
    void setName(const std::string& n) { name = n; }
    void setCollegeId(int cid) { collegeId = cid; }
};

#endif // MAJOR_H
