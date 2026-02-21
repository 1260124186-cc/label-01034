#ifndef COLLEGE_H
#define COLLEGE_H

#include <string>

class College {
private:
    int id;
    std::string name;
    std::string description;

public:
    College() : id(0) {}
    College(int id, const std::string& name, const std::string& description)
        : id(id), name(name), description(description) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }

    void setId(int id) { this->id = id; }
    void setName(const std::string& n) { name = n; }
    void setDescription(const std::string& d) { description = d; }
};

#endif // COLLEGE_H
