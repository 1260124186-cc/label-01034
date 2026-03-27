#include "DatabaseManager.h"
#include "UserManager.h"
#include "AuthProxy.h"
#include "CollegeManager.h"
#include "MajorManager.h"
#include "CourseManager.h"
#include "StudentManager.h"
#include "TeacherManager.h"
#include "ReportExporter.h"
#include "Logger.h"
#include <iostream>
#include <cassert>
#include <string>

using namespace std;

class TestRunner {
private:
    int passed = 0;
    int failed = 0;

    void printTestResult(const string& testName, bool result) {
        if (result) {
            cout << "[PASS] " << testName << endl;
            passed++;
        } else {
            cout << "[FAIL] " << testName << endl;
            failed++;
        }
    }

public:
    void runTest(const string& testName, bool (*testFunc)()) {
        cout << "\nRunning: " << testName << endl;
        try {
            bool result = testFunc();
            printTestResult(testName, result);
        } catch (const exception& e) {
            cout << "[EXCEPTION] " << testName << ": " << e.what() << endl;
            failed++;
        } catch (...) {
            cout << "[EXCEPTION] " << testName << ": Unknown exception" << endl;
            failed++;
        }
    }

    void printSummary() {
        cout << "\n========================================" << endl;
        cout << "Test Summary: " << endl;
        cout << "Passed: " << passed << endl;
        cout << "Failed: " << failed << endl;
        cout << "Total: " << passed + failed << endl;
        cout << "========================================" << endl;
    }
};

AuthProxy authProxy;
UserManager userMgr;
CollegeManager collegeMgr;
MajorManager majorMgr;
CourseManager courseMgr;
StudentManager studentMgr;
TeacherManager teacherMgr;

void resetDatabase() {
    DatabaseManager::destroyInstance();
    system("rm -f data/test_campus.db");
    DatabaseManager::getInstance()->open("data/test_campus.db");
}

bool testDatabaseConnection() {
    return DatabaseManager::getInstance()->getDB() != nullptr;
}

bool testAdminLogin() {
    Role role;
    int userId = userMgr.login("admin", "admin123", role);
    if (userId != -1) {
        authProxy.setCurrentUser(userId, role);
        return true;
    }
    return false;
}

bool testAddCollege() {
    return collegeMgr.addCollege("计算机学院", "计算机科学与技术学院");
}

bool testGetCollege() {
    College col = collegeMgr.getCollegeById(1);
    return col.getId() == 1 && col.getName() == "计算机学院";
}

bool testUpdateCollege() {
    return collegeMgr.updateCollege(1, "计算机科学学院", "新的描述");
}

bool testAddMajor() {
    return majorMgr.addMajor("软件工程", 1);
}

bool testGetMajor() {
    Major maj = majorMgr.getMajorById(1);
    return maj.getId() == 1 && maj.getName() == "软件工程";
}

bool testRegisterTeacher() {
    return userMgr.registerTeacher("teacher1", "Teacher123!", "张老师", "T001", 1);
}

bool testRegisterStudent() {
    return userMgr.registerStudent("student1", "Student123!", "李学生", "S001", 1);
}

bool testTeacherLogin() {
    authProxy.logout();
    Role role;
    int userId = userMgr.login("teacher1", "Teacher123!", role);
    if (userId != -1) {
        authProxy.setCurrentUser(userId, role);
        return true;
    }
    return false;
}

bool testAddCourse() {
    Teacher t = userMgr.getTeacherByUserId(authProxy.getCurrentUserId());
    return courseMgr.addCourse("C++程序设计", 3, "2025-2026-1", 50, t.getId(), "周一 1-2节");
}

bool testGetCourse() {
    Course c = courseMgr.getCourseById(1);
    return c.getId() == 1 && c.getName() == "C++程序设计";
}

bool testStudentLogin() {
    authProxy.logout();
    Role role;
    int userId = userMgr.login("student1", "Student123!", role);
    if (userId != -1) {
        authProxy.setCurrentUser(userId, role);
        return true;
    }
    return false;
}

bool testSelectCourse() {
    Student s = userMgr.getStudentByUserId(authProxy.getCurrentUserId());
    std::string errorMsg;
    return studentMgr.selectCourse(s.getId(), 1, errorMsg);
}

bool testGetSelectedCourses() {
    Student s = userMgr.getStudentByUserId(authProxy.getCurrentUserId());
    auto courses = studentMgr.getSelectedCourses(s.getId());
    return !courses.empty();
}

bool testDropCourse() {
    Student s = userMgr.getStudentByUserId(authProxy.getCurrentUserId());
    return studentMgr.dropCourse(s.getId(), 1);
}

bool testExportCoursesCSV() {
    auto courses = courseMgr.getAllCourses();
    return ReportExporter::exportCoursesToCSV(courses, "data/test_courses.csv");
}

bool testExportCoursesTXT() {
    auto courses = courseMgr.getAllCourses();
    return ReportExporter::exportCoursesToTXT(courses, "data/test_courses.txt");
}

bool testDeleteMajor() {
    authProxy.logout();
    Role role;
    int userId = userMgr.login("admin", "admin123", role);
    if (userId != -1) {
        authProxy.setCurrentUser(userId, role);
    }
    return majorMgr.deleteMajor(1);
}

bool testDeleteCollege() {
    return collegeMgr.deleteCollege(1);
}

int main() {
    cout << "========================================" << endl;
    cout << "  大学校园学院管理系统 - 自动化测试" << endl;
    cout << "========================================" << endl;

    resetDatabase();

    TestRunner runner;

    runner.runTest("数据库连接测试", testDatabaseConnection);
    runner.runTest("管理员登录测试", testAdminLogin);
    runner.runTest("添加学院测试", testAddCollege);
    runner.runTest("查询学院测试", testGetCollege);
    runner.runTest("更新学院测试", testUpdateCollege);
    runner.runTest("添加专业测试", testAddMajor);
    runner.runTest("查询专业测试", testGetMajor);
    runner.runTest("注册教师测试", testRegisterTeacher);
    runner.runTest("注册学生测试", testRegisterStudent);
    runner.runTest("教师登录测试", testTeacherLogin);
    runner.runTest("添加课程测试", testAddCourse);
    runner.runTest("查询课程测试", testGetCourse);
    runner.runTest("学生登录测试", testStudentLogin);
    runner.runTest("学生选课测试", testSelectCourse);
    runner.runTest("查询已选课程测试", testGetSelectedCourses);
    runner.runTest("学生退选测试", testDropCourse);
    runner.runTest("导出课程CSV测试", testExportCoursesCSV);
    runner.runTest("导出课程TXT测试", testExportCoursesTXT);
    runner.runTest("删除专业测试", testDeleteMajor);
    runner.runTest("删除学院测试", testDeleteCollege);

    runner.printSummary();

    DatabaseManager::destroyInstance();
    Logger::destroyInstance();

    return 0;
}
