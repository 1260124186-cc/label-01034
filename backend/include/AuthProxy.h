#ifndef AUTH_PROXY_H
#define AUTH_PROXY_H

#include "User.h"
#include <string>

// 代理模式 - 权限控制
class AuthProxy {
private:
    Role currentRole;
    int currentUserId;
    bool loggedIn;

public:
    AuthProxy();

    void setCurrentUser(int userId, Role role);
    void logout();

    bool isLoggedIn() const { return loggedIn; }
    Role getCurrentRole() const { return currentRole; }
    int getCurrentUserId() const { return currentUserId; }

    // 权限检查
    bool canManageCollege() const;    // 仅管理员
    bool canManageMajor() const;      // 仅管理员
    bool canManageUsers() const;      // 仅管理员
    bool canCreateCourse() const;     // 教师
    bool canSelectCourse() const;     // 学生
    bool canViewAllStudents() const;  // 管理员和教师
    bool canExportReport() const;     // 管理员和教师
};

#endif // AUTH_PROXY_H
