#ifndef COURSE_H
#define COURSE_H

#include <string>

class Course {
private:
    int id;
    std::string name;
    int credit;          // 学分
    std::string semester; // 学期
    int capacity;        // 容量
    int enrolled;        // 已选人数
    int teacherId;       // 授课教师ID
    std::string schedule; // 上课时间，格式: "周一1-2,周三3-4"

public:
    Course() : id(0), credit(0), capacity(0), enrolled(0), teacherId(0) {}
    Course(int id, const std::string& name, int credit, const std::string& semester,
           int capacity, int enrolled, int teacherId, const std::string& schedule)
        : id(id), name(name), credit(credit), semester(semester),
          capacity(capacity), enrolled(enrolled), teacherId(teacherId), schedule(schedule) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    int getCredit() const { return credit; }
    std::string getSemester() const { return semester; }
    int getCapacity() const { return capacity; }
    int getEnrolled() const { return enrolled; }
    int getTeacherId() const { return teacherId; }
    std::string getSchedule() const { return schedule; }

    void setId(int id) { this->id = id; }
    void setName(const std::string& n) { name = n; }
    void setCredit(int c) { credit = c; }
    void setSemester(const std::string& s) { semester = s; }
    void setCapacity(int c) { capacity = c; }
    void setEnrolled(int e) { enrolled = e; }
    void setTeacherId(int tid) { teacherId = tid; }
    void setSchedule(const std::string& s) { schedule = s; }

    bool isFull() const { return enrolled >= capacity; }
};

#endif // COURSE_H
