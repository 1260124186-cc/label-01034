#include "AuthProxy.h"

AuthProxy::AuthProxy() : currentRole(Role::STUDENT), currentUserId(-1), loggedIn(false) {}

void AuthProxy::setCurrentUser(int userId, Role role) {
    currentUserId = userId;
    currentRole = role;
    loggedIn = true;
}

void AuthProxy::logout() {
    currentUserId = -1;
    loggedIn = false;
}

bool AuthProxy::canManageCollege() const {
    return loggedIn && currentRole == Role::ADMIN;
}

bool AuthProxy::canManageMajor() const {
    return loggedIn && currentRole == Role::ADMIN;
}

bool AuthProxy::canManageUsers() const {
    return loggedIn && currentRole == Role::ADMIN;
}

bool AuthProxy::canCreateCourse() const {
    return loggedIn && currentRole == Role::TEACHER;
}

bool AuthProxy::canSelectCourse() const {
    return loggedIn && currentRole == Role::STUDENT;
}

bool AuthProxy::canViewAllStudents() const {
    return loggedIn && (currentRole == Role::ADMIN || currentRole == Role::TEACHER);
}

bool AuthProxy::canExportReport() const {
    return loggedIn && (currentRole == Role::ADMIN || currentRole == Role::TEACHER);
}
