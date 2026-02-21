# 大学校园学院管理系统

## How to Run

**环境要求：**
- g++（支持 C++17）
- SQLite3（macOS 可通过 `brew install sqlite` 安装）

**编译与运行：**

```bash
cd backend
make        # 编译
make run    # 编译并运行
make clean  # 清理编译产物和数据库
```

程序启动后会自动在 `backend/data/` 下创建 `campus.db` 数据库文件，并初始化表结构和默认管理员账号。

**使用流程：**

1. 启动程序后，使用管理员账号登录
2. 先创建学院，再创建专业（专业需关联学院ID）
3. 退出后注册教师账号（需填写所属学院ID）和学生账号（需填写所属专业ID）
4. 教师登录后可发布课程（设置学分、学期、容量、上课时间）
5. 学生登录后可浏览课程、选课、退选、查看已选课程
6. 管理员和教师可导出报表为 CSV/TXT 文件

## Services

| 服务 | 路径 | 说明 |
|------|------|------|
| backend | `backend/` | C++ 后端，基于 SQLite3 的本地化学院管理系统 |

## 测试账号

| 角色 | 用户名 | 密码 |
|------|--------|------|
| 管理员 | admin | admin123 |

> 教师和学生账号可通过系统注册功能自行创建。注册时需确保关联的学院ID/专业ID已存在。

## 题目内容

```
�
�⼤学校园学院管理系统

项⽬说明⽂档
⼀、项⽬背景
随着⾼校信息化管理的推进，传统的⼿⼯管理模式已⽆法满⾜现代⾼校对教学资源的⾼效利⽤。为了提升⾼校教学管理⽔平，开发⼀套本地化的
“
⼤学
校园学院管理系统
”
，⽤于管理学院、专业、教师、课程、学⽣及选课等信息，具有现实意义和良好的学习价值。
本系统不涉及⽹络通信与多线程编程，适合初学者在掌握⾯向对象编程、数据库操作、设计模式等知识点后进⾏综合训练

⼆、项⽬⽬标
开发⼀个本地化、功能完整、结构清晰的⼤学校园学院管理系统，⽀持：
学院与专业的管理
教师与学⽣的注册与管理
课程的创建与发布
学⽣选课与退选
教师查看所授课程与学⽣名单
系统管理员维护⽤⼾权限与数据安全
⽀持数据持久化存储（SQLite）
使⽤设计模式提⾼系统可扩展性与可维护

三、项⽬功能模块
模块功能描述
⽤⼾管理模块   	 实现登录、注册、⻆⾊识别（管理员、教师、学⽣）
学院管理模块 	 添加、修改、删除学院，管理下属专业
专业管理模块 	 添加、修改、删除专业，关联所属学院
教师管理模块 	 注册教师账号，分配授课课程
学⽣管理模块 	 注册学⽣账号，查看个⼈选课情况
课程管理模块 	 教师添加课程，设置课程属性（学分、学期、容量等）
选课管理模块 	 学⽣选课、退选，限制选课⼈数，避免冲突
报表导出模块 	 导出课程列表、学⽣选课记录为CSV 或TXT文件

四、技术实现要求
1. 编程语⾔与⼯具
语⾔：C++
数据库：SQLite3（使⽤ sqlite3 C/C++ 接⼝）
开发环境：VSCode + g++
数据结构：vector、map、set、⾃定义类等 STL 容器
⽂件操作：fstream ⽤于导出报表

2. 核⼼知识应⽤
技术点
⾯向对象编程 	 设计User、Student、Teacher、Course、College、Major 等类
单例模式 	 	 数据库连接管理器、配置中⼼、⽇志记录器
⼯⼚模式 	 	 创建不同类型的⽤⼾（学⽣、教师、管理员）
策略模式 	 	 实现不同的选课策略（如优先级、时间冲突检测）
代理模式 	 	 实现权限控制（如仅管理员可操作某些功能）
SQLite3 	 	 存储⽤⼾、课程、选课等数据
⽂件操作 	 	 导出选课记录、课程清单等为⽂本/CSV格式

五、系统架构设计（简要）
Main.cpp
UserManager.cpp/h
UserFactory.cpp/h （工厂模式）
AuthProxy.cpp/h （代理模式）
CollegeManager.cpp/h
College.cpp/h
MajorManager.cpp/h
Major.cpp/h
CourseManager.cpp/h
Course.cpp/h
SelectionStrategy.cpp/h （策略模式）
StudentManager.cpp/h
Student.cpp/h
TeacherManager.cpp/h
Teacher.cpp/h
DatabaseManager.cpp/h （单例模式）
ReportExporter.cpp/

六、详细功能描述

1. ⽤⼾登录与权限控制
⽀持三种⻆⾊：管理员、教师、学⽣
使⽤代理模式控制访问权限
登录成功后根据⻆⾊进⼊对应功能菜单

2. 学院与专业管理（管理员）
增删改查学院信息
增删改查专业信息，并绑定到学院
所有操作需写⼊数据库

3. 教师管理
教师注册与登录
教师可发布课程、查看所授课程、查看选课学⽣名单

4. 学⽣管理
学⽣注册与登录
浏览所有课程
选课与退选（需判断是否已选满或时间冲突）
查看⾃⼰已选课程

5. 课程管理
教师可创建课程（课程名、学分、学期、容量、上课时间等）
⽀持策略模式定义不同的选课规则（如先到先得、按年级排序等）

6. 选课逻辑处理
判断选课⼈数是否超限
时间冲突检测（如两个课程在同⼀时间段）
选课记录存⼊数据库

7. 数据持久
所有实体信息保存⾄SQLite 数据库中
表结构包括：
users, students, teachers, colleges, majors, courses, course_selections

8. 报表导出功能
⽀持将课程列表、学⽣选课记录导出为CSV 或TXT ⽂件
使⽤fstream 进⾏⽂件写⼊

是否满足以上要求
```

## 项目结构

```
.
├── .gitignore
├── README.md
└── backend/
    ├── Makefile                # 编译配置
    ├── include/                # 头文件
    │   ├── AuthProxy.h         # 权限代理（代理模式）
    │   ├── College.h           # 学院类
    │   ├── CollegeManager.h    # 学院管理
    │   ├── Course.h            # 课程类
    │   ├── CourseManager.h     # 课程管理
    │   ├── DatabaseManager.h   # 数据库管理（单例模式）
    │   ├── InputValidator.h    # 输入校验工具
    │   ├── Logger.h            # 日志记录器（单例模式）
    │   ├── Major.h             # 专业类
    │   ├── MajorManager.h      # 专业管理
    │   ├── ReportExporter.h    # 报表导出
    │   ├── SHA256Util.h        # SHA256哈希工具
    │   ├── SelectionStrategy.h # 选课策略（策略模式）
    │   ├── Student.h           # 学生类（继承User）
    │   ├── StudentManager.h    # 学生选课管理
    │   ├── Teacher.h           # 教师类（继承User）
    │   ├── TeacherManager.h    # 教师管理
    │   ├── User.h              # 用户基类
    │   ├── UserFactory.h       # 用户工厂（工厂模式）
    │   └── UserManager.h       # 用户管理
    ├── src/                    # 源文件（与头文件一一对应）
    │   ├── AuthProxy.cpp
    │   ├── CollegeManager.cpp
    │   ├── CourseManager.cpp
    │   ├── DatabaseManager.cpp
    │   ├── InputValidator.cpp
    │   ├── Logger.cpp
    │   ├── Main.cpp            # 主程序入口与菜单交互
    │   ├── MajorManager.cpp
    │   ├── ReportExporter.cpp
    │   ├── SHA256Util.cpp
    │   ├── SelectionStrategy.cpp
    │   ├── StudentManager.cpp
    │   ├── TeacherManager.cpp
    │   ├── UserFactory.cpp
    │   └── UserManager.cpp
    ├── build/                  # 编译产物（自动生成）
    └── data/                   # 运行时数据（自动生成）
        ├── campus.db           # SQLite 数据库
        ├── system.log          # 系统日志
        └── *.csv / *.txt       # 导出的报表文件
```

**功能模块：**

| 模块 | 说明 |
|------|------|
| 用户管理 | 登录、注册、角色识别（管理员/教师/学生），输入自动 trim，密码 SHA256 哈希存储 |
| 学院管理 | 增删改查学院信息（管理员），含存在性校验 |
| 专业管理 | 增删改查专业，关联所属学院（管理员），含存在性校验 |
| 教师管理 | 注册教师、发布课程、查看选课学生名单 |
| 学生管理 | 注册学生、浏览课程、选课/退选、查看已选 |
| 课程管理 | 创建课程（学分、学期、容量、上课时间） |
| 选课管理 | 容量限制、时间冲突检测（组合策略） |
| 报表导出 | 导出课程列表、选课记录为 CSV/TXT |

**设计模式：**

| 模式 | 实现 | 说明 |
|------|------|------|
| 单例模式 | `DatabaseManager` | 全局唯一数据库连接，统一管理生命周期 |
| 单例模式 | `Logger` | 全局日志记录器，支持日志级别（DEBUG/INFO/WARN/ERROR/FATAL）、文件输出、时间戳 |
| 工厂模式 | `UserFactory` | 根据角色创建 Student/Teacher/Admin 对象 |
| 策略模式 | `SelectionStrategy` | FirstComeStrategy（容量检查）+ TimeConflictStrategy（时间冲突）+ CompositeStrategy（组合） |
| 代理模式 | `AuthProxy` | 根据当前登录角色控制功能访问权限 |

**数据库表结构：**

| 表名 | 说明 |
|------|------|
| users | 用户表（id, username, password, name, role） |
| students | 学生表（id, user_id, student_no, major_id） |
| teachers | 教师表（id, user_id, teacher_no, college_id） |
| colleges | 学院表（id, name, description） |
| majors | 专业表（id, name, college_id） |
| courses | 课程表（id, name, credit, semester, capacity, enrolled, teacher_id, schedule） |
| course_selections | 选课记录表（id, student_id, course_id, select_time） |
