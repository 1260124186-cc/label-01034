#include "DatabaseManager.h"
#include "UserManager.h"
#include "UserFactory.h"
#include "AuthProxy.h"
#include "CollegeManager.h"
#include "MajorManager.h"
#include "CourseManager.h"
#include "StudentManager.h"
#include "TeacherManager.h"
#include "ReportExporter.h"
#include "Student.h"
#include "Teacher.h"
#include "Logger.h"
#include "InputValidator.h"

#include <iostream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

// 全局管理器
AuthProxy authProxy;
UserManager userMgr;
CollegeManager collegeMgr;
MajorManager majorMgr;
CourseManager courseMgr;
StudentManager studentMgr;
TeacherManager teacherMgr;

// 清除输入缓冲
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// 暂停
void pause() {
    cout << "\n按回车键继续...";
    clearInput();
}

// 前置声明
void loginMenu();
void adminMenu();
void teacherMenu();
void studentMenu();
void studentViewSelected();

// ==================== 管理员功能 ====================

void manageColleges() {
    if (!authProxy.canManageCollege()) {
        cout << "权限不足！" << endl;
        return;
    }
    int choice;
    do {
        cout << "\n--- 学院管理 ---" << endl;
        cout << "1. 查看所有学院" << endl;
        cout << "2. 添加学院" << endl;
        cout << "3. 修改学院" << endl;
        cout << "4. 删除学院" << endl;
        cout << "0. 返回" << endl;
        cout << "请选择: ";
        cin >> choice;

        if (choice == 1) {
            auto colleges = collegeMgr.getAllColleges();
            if (colleges.empty()) {
                cout << "暂无学院信息" << endl;
            } else {
                cout << "\nID\t学院名称\t\t描述" << endl;
                for (const auto& c : colleges) {
                    cout << c.getId() << "\t" << c.getName() << "\t\t" << c.getDescription() << endl;
                }
            }
        } else if (choice == 2) {
            string name, desc;
            cout << "请输入学院名称: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, name);
            cout << "请输入学院描述: ";
            getline(cin, desc);
            if (collegeMgr.addCollege(name, desc))
                cout << "添加成功！" << endl;
            else
                cout << "添加失败（可能名称重复）" << endl;
        } else if (choice == 3) {
            int id;
            string name, desc;
            cout << "请输入要修改的学院ID: ";
            cin >> id;
            // 检查学院是否存在
            College existing = collegeMgr.getCollegeById(id);
            if (existing.getId() == 0) {
                cout << "学院不存在！" << endl;
                continue;
            }
            cout << "请输入新名称: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, name);
            cout << "请输入新描述: ";
            getline(cin, desc);
            if (collegeMgr.updateCollege(id, name, desc))
                cout << "修改成功！" << endl;
            else
                cout << "修改失败" << endl;
        } else if (choice == 4) {
            int id;
            cout << "请输入要删除的学院ID: ";
            cin >> id;
            College existing = collegeMgr.getCollegeById(id);
            if (existing.getId() == 0) {
                cout << "学院不存在！" << endl;
                continue;
            }
            if (collegeMgr.hasRelatedMajors(id)) {
                cout << "该学院下存在关联的专业，无法删除！请先删除相关专业。" << endl;
                continue;
            }
            if (collegeMgr.hasRelatedTeachers(id)) {
                cout << "该学院下存在关联的教师，无法删除！请先处理相关教师。" << endl;
                continue;
            }
            if (collegeMgr.deleteCollege(id))
                cout << "删除成功！" << endl;
            else
                cout << "删除失败" << endl;
        }
    } while (choice != 0);
}

void manageMajors() {
    if (!authProxy.canManageMajor()) {
        cout << "权限不足！" << endl;
        return;
    }
    int choice;
    do {
        cout << "\n--- 专业管理 ---" << endl;
        cout << "1. 查看所有专业" << endl;
        cout << "2. 添加专业" << endl;
        cout << "3. 修改专业" << endl;
        cout << "4. 删除专业" << endl;
        cout << "5. 按学院查看专业" << endl;
        cout << "0. 返回" << endl;
        cout << "请选择: ";
        cin >> choice;

        if (choice == 1) {
            auto majors = majorMgr.getAllMajors();
            if (majors.empty()) {
                cout << "暂无专业信息" << endl;
            } else {
                cout << "\nID\t专业名称\t\t所属学院ID" << endl;
                for (const auto& m : majors) {
                    cout << m.getId() << "\t" << m.getName() << "\t\t" << m.getCollegeId() << endl;
                }
            }
        } else if (choice == 2) {
            string name;
            int collegeId;
            cout << "请输入专业名称: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, name);
            cout << "请输入所属学院ID: ";
            cin >> collegeId;
            if (majorMgr.addMajor(name, collegeId))
                cout << "添加成功！" << endl;
            else
                cout << "添加失败" << endl;
        } else if (choice == 3) {
            int id, collegeId;
            string name;
            cout << "请输入要修改的专业ID: ";
            cin >> id;
            // 检查专业是否存在
            Major existingMajor = majorMgr.getMajorById(id);
            if (existingMajor.getId() == 0) {
                cout << "专业不存在！" << endl;
                continue;
            }
            cout << "请输入新名称: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, name);
            cout << "请输入新的所属学院ID: ";
            cin >> collegeId;
            if (majorMgr.updateMajor(id, name, collegeId))
                cout << "修改成功！" << endl;
            else
                cout << "修改失败" << endl;
        } else if (choice == 4) {
            int id;
            cout << "请输入要删除的专业ID: ";
            cin >> id;
            Major existingMajor = majorMgr.getMajorById(id);
            if (existingMajor.getId() == 0) {
                cout << "专业不存在！" << endl;
                continue;
            }
            if (majorMgr.hasRelatedStudents(id)) {
                cout << "该专业下存在关联的学生，无法删除！请先处理相关学生。" << endl;
                continue;
            }
            if (majorMgr.deleteMajor(id))
                cout << "删除成功！" << endl;
            else
                cout << "删除失败" << endl;
        } else if (choice == 5) {
            int collegeId;
            cout << "请输入学院ID: ";
            cin >> collegeId;
            // 检查学院是否存在
            College col = collegeMgr.getCollegeById(collegeId);
            if (col.getId() == 0) {
                cout << "学院不存在！" << endl;
                continue;
            }
            auto majors = majorMgr.getMajorsByCollege(collegeId);
            if (majors.empty()) {
                cout << "该学院下暂无专业" << endl;
            } else {
                cout << "\nID\t专业名称" << endl;
                for (const auto& m : majors) {
                    cout << m.getId() << "\t" << m.getName() << endl;
                }
            }
        }
    } while (choice != 0);
}

void manageUsers() {
    if (!authProxy.canManageUsers()) {
        cout << "权限不足！" << endl;
        return;
    }
    int choice;
    do {
        cout << "\n--- 用户管理 ---" << endl;
        cout << "1. 查看所有用户" << endl;
        cout << "2. 修改用户姓名" << endl;
        cout << "3. 重置用户密码" << endl;
        cout << "4. 删除用户" << endl;
        cout << "0. 返回" << endl;
        cout << "请选择: ";
        cin >> choice;

        if (choice == 1) {
            auto users = userMgr.getAllUsers();
            cout << "\nID\t用户名\t\t姓名\t\t角色" << endl;
            for (const auto& u : users) {
                cout << u.getId() << "\t" << u.getUsername() << "\t\t"
                     << u.getName() << "\t\t" << u.getRoleString() << endl;
            }
        } else if (choice == 2) {
            int uid;
            string newName;
            cout << "请输入用户ID: ";
            cin >> uid;
            User u = userMgr.getUserById(uid);
            if (u.getId() == 0) {
                cout << "用户不存在！" << endl;
                continue;
            }
            cout << "当前姓名: " << u.getName() << endl;
            cout << "请输入新姓名: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, newName);
            if (!InputValidator::isNameValid(newName)) {
                cout << "姓名不能为空且不超过50个字符" << endl;
                continue;
            }
            if (userMgr.updateUserName(uid, newName))
                cout << "修改成功！" << endl;
            else
                cout << "修改失败" << endl;
        } else if (choice == 3) {
            int uid;
            string newPass;
            cout << "请输入用户ID: ";
            cin >> uid;
            User u = userMgr.getUserById(uid);
            if (u.getId() == 0) {
                cout << "用户不存在！" << endl;
                continue;
            }
            cout << "用户: " << u.getUsername() << " (" << u.getRoleString() << ")" << endl;
            cout << "请输入新密码(" << InputValidator::getPasswordRule() << "): ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, newPass);
            if (!InputValidator::isPasswordValid(newPass)) {
                cout << "密码格式不正确！" << InputValidator::getPasswordRule() << endl;
                continue;
            }
            if (userMgr.resetPassword(uid, newPass))
                cout << "密码重置成功！" << endl;
            else
                cout << "密码重置失败" << endl;
        } else if (choice == 4) {
            int uid;
            cout << "请输入要删除的用户ID: ";
            cin >> uid;
            User u = userMgr.getUserById(uid);
            if (u.getId() == 0) {
                cout << "用户不存在！" << endl;
                continue;
            }
            if (uid == authProxy.getCurrentUserId()) {
                cout << "不能删除当前登录的管理员账号！" << endl;
                continue;
            }
            // 如果是教师，检查是否有关联课程
            if (u.getRole() == Role::TEACHER) {
                Teacher t = userMgr.getTeacherByUserId(uid);
                auto courses = courseMgr.getCoursesByTeacher(t.getId());
                if (!courses.empty()) {
                    cout << "该教师有 " << courses.size() << " 门关联课程，无法删除！请先删除其课程。" << endl;
                    continue;
                }
            }
            cout << "确认删除用户 [" << u.getUsername() << " - " << u.getName()
                 << " - " << u.getRoleString() << "]？(y/n): ";
            char confirm;
            cin >> confirm;
            if (confirm == 'y' || confirm == 'Y') {
                if (userMgr.deleteUser(uid))
                    cout << "删除成功！" << endl;
                else
                    cout << "删除失败" << endl;
            } else {
                cout << "已取消" << endl;
            }
        }
    } while (choice != 0);
}

void adminExportReport() {
    if (!authProxy.canExportReport()) {
        cout << "权限不足！" << endl;
        return;
    }
    cout << "\n--- 报表导出 ---" << endl;
    cout << "1. 导出课程列表(CSV)" << endl;
    cout << "2. 导出课程列表(TXT)" << endl;
    cout << "请选择: ";
    int choice;
    cin >> choice;

    auto courses = courseMgr.getAllCourses();
    if (choice == 1) {
        if (ReportExporter::exportCoursesToCSV(courses, "data/courses_report.csv"))
            cout << "已导出到 data/courses_report.csv" << endl;
        else
            cout << "导出失败" << endl;
    } else if (choice == 2) {
        if (ReportExporter::exportCoursesToTXT(courses, "data/courses_report.txt"))
            cout << "已导出到 data/courses_report.txt" << endl;
        else
            cout << "导出失败" << endl;
    }
}

void adminMenu() {
    int choice;
    do {
        cout << "\n========== 管理员菜单 ==========" << endl;
        cout << "1. 学院管理" << endl;
        cout << "2. 专业管理" << endl;
        cout << "3. 查看所有用户" << endl;
        cout << "4. 查看所有课程" << endl;
        cout << "5. 导出报表" << endl;
        cout << "0. 退出登录" << endl;
        cout << "请选择: ";
        cin >> choice;

        switch (choice) {
            case 1: manageColleges(); break;
            case 2: manageMajors(); break;
            case 3: manageUsers(); break;
            case 4: {
                auto courses = courseMgr.getAllCourses();
                if (courses.empty()) {
                    cout << "暂无课程" << endl;
                } else {
                    cout << "\nID\t课程名称\t学分\t学期\t容量\t已选\t时间" << endl;
                    for (const auto& c : courses) {
                        cout << c.getId() << "\t" << c.getName() << "\t"
                             << c.getCredit() << "\t" << c.getSemester() << "\t"
                             << c.getCapacity() << "\t" << c.getEnrolled() << "\t"
                             << c.getSchedule() << endl;
                    }
                }
                break;
            }
            case 5: adminExportReport(); break;
            case 0: authProxy.logout(); cout << "已退出登录" << endl; break;
            default: cout << "无效选择" << endl;
        }
    } while (choice != 0);
}

// ==================== 教师功能 ====================

void teacherViewCourses() {
    Teacher t = userMgr.getTeacherByUserId(authProxy.getCurrentUserId());
    auto courses = courseMgr.getCoursesByTeacher(t.getId());
    if (courses.empty()) {
        cout << "您暂未发布任何课程" << endl;
        return;
    }
    cout << "\nID\t课程名称\t学分\t学期\t容量\t已选\t时间" << endl;
    for (const auto& c : courses) {
        cout << c.getId() << "\t" << c.getName() << "\t"
             << c.getCredit() << "\t" << c.getSemester() << "\t"
             << c.getCapacity() << "\t" << c.getEnrolled() << "\t"
             << c.getSchedule() << endl;
    }
}

void teacherAddCourse() {
    if (!authProxy.canCreateCourse()) {
        cout << "权限不足！" << endl;
        return;
    }
    Teacher t = userMgr.getTeacherByUserId(authProxy.getCurrentUserId());

    string name, semester, schedule;
    int credit, capacity;

    cout << "请输入课程名称: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);
    if (!InputValidator::isNameValid(name)) {
        cout << "课程名称不能为空" << endl;
        return;
    }
    cout << "请输入学分(1-10): ";
    cin >> credit;
    if (!InputValidator::isCreditValid(credit)) {
        cout << "学分必须在1-10之间" << endl;
        return;
    }
    cout << "请输入学期(如 2025-2026-1): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, semester);
    if (semester.empty()) {
        cout << "学期不能为空" << endl;
        return;
    }
    cout << "请输入课程容量(1-500): ";
    cin >> capacity;
    if (!InputValidator::isCapacityValid(capacity)) {
        cout << "课程容量必须在1-500之间" << endl;
        return;
    }
    cout << "请输入上课时间(" << InputValidator::getScheduleRule() << "): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, schedule);
    if (!InputValidator::isScheduleValid(schedule)) {
        cout << "时间格式不正确！" << InputValidator::getScheduleRule() << endl;
        return;
    }

    if (courseMgr.addCourse(name, credit, semester, capacity, t.getId(), schedule))
        cout << "课程创建成功！" << endl;
    else
        cout << "课程创建失败" << endl;
}

void teacherViewStudents() {
    Teacher t = userMgr.getTeacherByUserId(authProxy.getCurrentUserId());
    auto courses = courseMgr.getCoursesByTeacher(t.getId());
    if (courses.empty()) {
        cout << "您暂未发布任何课程" << endl;
        return;
    }

    cout << "您的课程列表:" << endl;
    for (const auto& c : courses) {
        cout << "  ID: " << c.getId() << "  " << c.getName() << endl;
    }

    int courseId;
    cout << "请输入要查看学生名单的课程ID: ";
    cin >> courseId;

    auto students = teacherMgr.getStudentsByCourse(courseId);
    if (students.empty()) {
        cout << "该课程暂无学生选课" << endl;
    } else {
        cout << "\n学号\t\t姓名\t\t专业ID" << endl;
        for (const auto& s : students) {
            cout << s.getStudentNo() << "\t\t" << s.getName() << "\t\t" << s.getMajorId() << endl;
        }
    }
}

void teacherExportReport() {
    if (!authProxy.canExportReport()) {
        cout << "权限不足！" << endl;
        return;
    }
    Teacher t = userMgr.getTeacherByUserId(authProxy.getCurrentUserId());
    auto courses = courseMgr.getCoursesByTeacher(t.getId());

    cout << "\n--- 导出报表 ---" << endl;
    cout << "1. 导出我的课程列表(CSV)" << endl;
    cout << "2. 导出某课程学生名单(CSV)" << endl;
    cout << "请选择: ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        if (ReportExporter::exportCoursesToCSV(courses, "data/my_courses.csv"))
            cout << "已导出到 data/my_courses.csv" << endl;
        else
            cout << "导出失败" << endl;
    } else if (choice == 2) {
        int courseId;
        cout << "请输入课程ID: ";
        cin >> courseId;
        if (ReportExporter::exportCourseStudentsToCSV(courseId, "data/course_students.csv"))
            cout << "已导出到 data/course_students.csv" << endl;
        else
            cout << "导出失败" << endl;
    }
}

void teacherMenu() {
    int choice;
    do {
        cout << "\n========== 教师菜单 ==========" << endl;
        cout << "1. 查看我的课程" << endl;
        cout << "2. 发布新课程" << endl;
        cout << "3. 查看选课学生名单" << endl;
        cout << "4. 导出报表" << endl;
        cout << "0. 退出登录" << endl;
        cout << "请选择: ";
        cin >> choice;

        switch (choice) {
            case 1: teacherViewCourses(); break;
            case 2: teacherAddCourse(); break;
            case 3: teacherViewStudents(); break;
            case 4: teacherExportReport(); break;
            case 0: authProxy.logout(); cout << "已退出登录" << endl; break;
            default: cout << "无效选择" << endl;
        }
    } while (choice != 0);
}

// ==================== 学生功能 ====================

void studentBrowseCourses() {
    auto courses = courseMgr.getAllCourses();
    if (courses.empty()) {
        cout << "暂无可选课程" << endl;
        return;
    }
    cout << "\nID\t课程名称\t学分\t学期\t容量\t已选\t时间" << endl;
    for (const auto& c : courses) {
        cout << c.getId() << "\t" << c.getName() << "\t"
             << c.getCredit() << "\t" << c.getSemester() << "\t"
             << c.getCapacity() << "\t" << c.getEnrolled() << "\t"
             << c.getSchedule() << endl;
    }
}

void studentSelectCourse() {
    if (!authProxy.canSelectCourse()) {
        cout << "权限不足！" << endl;
        return;
    }
    Student s = userMgr.getStudentByUserId(authProxy.getCurrentUserId());

    studentBrowseCourses();
    int courseId;
    cout << "\n请输入要选的课程ID: ";
    cin >> courseId;

    string errorMsg;
    if (studentMgr.selectCourse(s.getId(), courseId, errorMsg))
        cout << "选课成功！" << endl;
    else
        cout << "选课失败: " << errorMsg << endl;
}

void studentDropCourse() {
    if (!authProxy.canSelectCourse()) {
        cout << "权限不足！" << endl;
        return;
    }
    Student s = userMgr.getStudentByUserId(authProxy.getCurrentUserId());

    studentViewSelected();
    int courseId;
    cout << "\n请输入要退选的课程ID: ";
    cin >> courseId;

    if (studentMgr.dropCourse(s.getId(), courseId))
        cout << "退选成功！" << endl;
    else
        cout << "退选失败（可能未选该课程）" << endl;
}

void studentViewSelected() {
    Student s = userMgr.getStudentByUserId(authProxy.getCurrentUserId());
    auto courses = studentMgr.getSelectedCourses(s.getId());
    if (courses.empty()) {
        cout << "您暂未选择任何课程" << endl;
        return;
    }
    cout << "\n--- 我的已选课程 ---" << endl;
    cout << "ID\t课程名称\t学分\t学期\t时间" << endl;
    for (const auto& c : courses) {
        cout << c.getId() << "\t" << c.getName() << "\t"
             << c.getCredit() << "\t" << c.getSemester() << "\t"
             << c.getSchedule() << endl;
    }
}

void studentMenu() {
    int choice;
    do {
        cout << "\n========== 学生菜单 ==========" << endl;
        cout << "1. 浏览所有课程" << endl;
        cout << "2. 选课" << endl;
        cout << "3. 退选" << endl;
        cout << "4. 查看已选课程" << endl;
        cout << "5. 导出我的选课记录(CSV)" << endl;
        cout << "0. 退出登录" << endl;
        cout << "请选择: ";
        cin >> choice;

        switch (choice) {
            case 1: studentBrowseCourses(); break;
            case 2: studentSelectCourse(); break;
            case 3: studentDropCourse(); break;
            case 4: studentViewSelected(); break;
            case 5: {
                Student s = userMgr.getStudentByUserId(authProxy.getCurrentUserId());
                if (ReportExporter::exportSelectionsToCSV(s.getId(), "data/my_selections.csv"))
                    cout << "已导出到 data/my_selections.csv" << endl;
                else
                    cout << "导出失败" << endl;
                break;
            }
            case 0: authProxy.logout(); cout << "已退出登录" << endl; break;
            default: cout << "无效选择" << endl;
        }
    } while (choice != 0);
}

// ==================== 登录/注册 ====================

void registerUser() {
    cout << "\n--- 用户注册 ---" << endl;
    cout << "1. 注册学生" << endl;
    cout << "2. 注册教师" << endl;
    cout << "请选择: ";
    int choice;
    cin >> choice;

    string username, password, name;
    cout << "请输入用户名(" << InputValidator::getUsernameRule() << "): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, username);
    if (!InputValidator::isUsernameValid(username)) {
        cout << "用户名格式不正确！" << InputValidator::getUsernameRule() << endl;
        return;
    }
    cout << "请输入密码(" << InputValidator::getPasswordRule() << "): ";
    getline(cin, password);
    if (!InputValidator::isPasswordValid(password)) {
        cout << "密码格式不正确！" << InputValidator::getPasswordRule() << endl;
        return;
    }
    cout << "请输入姓名: ";
    getline(cin, name);
    if (!InputValidator::isNameValid(name)) {
        cout << "姓名不能为空且不超过50个字符" << endl;
        return;
    }

    if (choice == 1) {
        string studentNo;
        int majorId;
        cout << "请输入学号(" << InputValidator::getStudentNoRule() << "): ";
        getline(cin, studentNo);
        if (!InputValidator::isStudentNoValid(studentNo)) {
            cout << "学号格式不正确！" << InputValidator::getStudentNoRule() << endl;
            return;
        }
        cout << "请输入专业ID: ";
        cin >> majorId;
        if (!InputValidator::isPositiveInt(majorId)) {
            cout << "专业ID必须为正整数" << endl;
            return;
        }
        if (userMgr.registerStudent(username, password, name, studentNo, majorId))
            cout << "学生注册成功！" << endl;
        else
            cout << "注册失败（用户名或学号可能重复）" << endl;
    } else if (choice == 2) {
        string teacherNo;
        int collegeId;
        cout << "请输入工号(" << InputValidator::getTeacherNoRule() << "): ";
        getline(cin, teacherNo);
        if (!InputValidator::isTeacherNoValid(teacherNo)) {
            cout << "工号格式不正确！" << InputValidator::getTeacherNoRule() << endl;
            return;
        }
        cout << "请输入所属学院ID: ";
        cin >> collegeId;
        if (!InputValidator::isPositiveInt(collegeId)) {
            cout << "学院ID必须为正整数" << endl;
            return;
        }
        if (userMgr.registerTeacher(username, password, name, teacherNo, collegeId))
            cout << "教师注册成功！" << endl;
        else
            cout << "注册失败（用户名或工号可能重复）" << endl;
    } else {
        cout << "无效选择" << endl;
    }
}

void loginMenu() {
    int choice;
    do {
        cout << "\n--- 登录/注册 ---" << endl;
        cout << "1. 登录" << endl;
        cout << "2. 注册" << endl;
        cout << "0. 退出系统" << endl;
        cout << "请选择: ";
        cin >> choice;

        if (choice == 1) {
            string username, password;
            cout << "用户名: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, username);
            cout << "密码: ";
            getline(cin, password);

            Role role;
            int userId = userMgr.login(username, password, role);
            if (userId < 0) {
                cout << "登录失败，用户名或密码错误" << endl;
            } else {
                authProxy.setCurrentUser(userId, role);
                cout << "登录成功！欢迎您，";
                switch (role) {
                    case Role::ADMIN:
                        cout << "管理员" << endl;
                        adminMenu();
                        break;
                    case Role::TEACHER:
                        cout << "教师" << endl;
                        teacherMenu();
                        break;
                    case Role::STUDENT:
                        cout << "学生" << endl;
                        studentMenu();
                        break;
                }
            }
        } else if (choice == 2) {
            registerUser();
        }
    } while (choice != 0);
}

int main() {
    // 初始化日志系统
    Logger::getInstance()->init("data/system.log", LogLevel::DEBUG, true);
    Logger::getInstance()->info("系统启动", "Main");

    // 初始化数据库
    DatabaseManager::getInstance()->open("data/campus.db");

    cout << "========================================" << endl;
    cout << "    欢迎使用大学校园学院管理系统" << endl;
    cout << "========================================" << endl;

    loginMenu();

    Logger::getInstance()->info("系统关闭", "Main");
    DatabaseManager::destroyInstance();
    Logger::destroyInstance();
    cout << "感谢使用，再见！" << endl;
    return 0;
}
