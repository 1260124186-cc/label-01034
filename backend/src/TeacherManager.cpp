#include "TeacherManager.h"
#include "DatabaseManager.h"
#include <sqlite3.h>

std::vector<Student> TeacherManager::getStudentsByCourse(int courseId) {
    std::vector<Student> students;
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    sqlite3_stmt* stmt;
    std::string sql = R"(
        SELECT u.id, u.username, u.name, s.student_no, s.major_id, s.id
        FROM students s
        JOIN users u ON s.user_id = u.id
        JOIN course_selections cs ON s.id = cs.student_id
        WHERE cs.course_id = ?;
    )";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, courseId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Student st;
        st.setId(sqlite3_column_int(stmt, 5));
        st.setUsername(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        st.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        st.setStudentNo(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        st.setMajorId(sqlite3_column_int(stmt, 4));
        st.setRole(Role::STUDENT);
        students.push_back(st);
    }
    sqlite3_finalize(stmt);
    return students;
}
