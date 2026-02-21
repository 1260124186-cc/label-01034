#ifndef USER_H
#define USER_H

#include <string>

// 用户角色枚举
enum class Role {
    ADMIN,
    TEACHER,
    STUDENT
};

// 用户基类
class User {
protected:
    int id;
    std::string username;
    std::string password;
    std::string name;
    Role role;

public:
    User() : id(0), role(Role::STUDENT) {}
    User(int id, const std::string& username, const std::string& password,
         const std::string& name, Role role)
        : id(id), username(username), password(password), name(name), role(role) {}
    virtual ~User() = default;

    int getId() const { return id; }
    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }
    std::string getName() const { return name; }
    Role getRole() const { return role; }

    void setId(int id) { this->id = id; }
    void setUsername(const std::string& u) { username = u; }
    void setPassword(const std::string& p) { password = p; }
    void setName(const std::string& n) { name = n; }
    void setRole(Role r) { role = r; }

    virtual std::string getRoleString() const {
        switch (role) {
            case Role::ADMIN: return "管理员";
            case Role::TEACHER: return "教师";
            case Role::STUDENT: return "学生";
            default: return "未知";
        }
    }
};

#endif // USER_H
