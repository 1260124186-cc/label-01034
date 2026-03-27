#!/usr/bin/env python3
import subprocess
import time
import os
import sys

def run_command(cmd, cwd=None, timeout=10):
    """运行命令并返回输出"""
    try:
        result = subprocess.run(
            cmd,
            cwd=cwd,
            shell=True,
            capture_output=True,
            text=True,
            timeout=timeout
        )
        return result.returncode, result.stdout, result.stderr
    except subprocess.TimeoutExpired:
        return -1, "", "命令执行超时"

def clean_database():
    """清理数据库"""
    print("=== 清理数据库 ===")
    returncode, stdout, stderr = run_command("make clean", cwd="backend")
    if returncode != 0:
        print(f"清理失败: {stderr}")
        return False
    print("数据库已清理")
    return True

def compile_project():
    """编译项目"""
    print("\n=== 编译项目 ===")
    returncode, stdout, stderr = run_command("make", cwd="backend")
    if returncode != 0:
        print(f"编译失败: {stderr}")
        return False
    print("编译成功")
    return True

def run_test_case(name, input_sequence, expected_outputs, cwd="backend"):
    """运行一个测试用例"""
    print(f"\n=== 测试: {name} ===")

    # 构建命令
    cmd = "./campus_system"

    # 启动进程
    proc = subprocess.Popen(
        cmd,
        cwd=cwd,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )

    # 发送输入 - 确保每个输入后有足够的换行
    input_str = "\n".join(input_sequence) + "\n0\n0\n"  # 增加额外的退出命令
    try:
        stdout, stderr = proc.communicate(input=input_str, timeout=20)  # 增加超时时间
    except subprocess.TimeoutExpired:
        proc.kill()
        stdout, stderr = proc.communicate()
        print("测试超时")
        print(f"输出:\n{stdout}")
        print(f"错误:\n{stderr}")
        return False

    # 检查输出
    success = True
    for expected in expected_outputs:
        if expected not in stdout and expected not in stderr:
            print(f"❌ 预期输出未找到: {expected}")
            success = False

    if success:
        print("✅ 测试通过")
    else:
        print(f"输出:\n{stdout}")
        print(f"错误:\n{stderr}")

    return success

def main():
    print("=" * 60)
    print("大学校园学院管理系统 - 自动化测试")
    print("=" * 60)

    # 检查是否在正确的目录
    if not os.path.exists("backend"):
        print("错误: 找不到backend目录，请在项目根目录运行此脚本")
        sys.exit(1)

    # 1. 清理数据库
    if not clean_database():
        sys.exit(1)

    # 2. 编译项目
    if not compile_project():
        sys.exit(1)

    # 3. 运行各个测试用例
    test_cases = [
        {
            "name": "管理员登录",
            "input_sequence": ["1", "admin", "admin123", "0"],
            "expected_outputs": ["管理员菜单", "已退出登录"]
        },
        {
            "name": "创建学院和专业",
            "input_sequence": [
                "1", "admin", "admin123",  # 登录
                "1", "2", "计算机学院", "计算机科学与技术学院",  # 创建学院
                "0", "2", "2", "软件工程", "1",  # 返回主菜单 -> 专业管理 -> 创建专业
                "0", "0"  # 退出
            ],
            "expected_outputs": ["添加成功！", "专业名称"]
        },
        {
            "name": "注册教师",
            "input_sequence": [
                "2", "1",  # 注册 -> 教师
                "teacher1", "Teacher Zhang", "123456", "T001", "1",  # 教师信息
                "1", "teacher1", "123456", "0"  # 登录验证
            ],
            "expected_outputs": ["注册成功", "教师菜单"]
        },
        {
            "name": "注册学生",
            "input_sequence": [
                "2", "2",  # 注册 -> 学生
                "student1", "Student Li", "123456", "S2024001", "1",  # 学生信息
                "1", "student1", "123456", "0"  # 登录验证
            ],
            "expected_outputs": ["注册成功", "学生菜单"]
        },
        {
            "name": "教师创建课程",
            "input_sequence": [
                "1", "teacher1", "123456",  # 教师登录
                "2", "数据结构", "3", "2024-2025-1", "50", "周一1-2,周三3-4",  # 创建课程
                "0", "0"
            ],
            "expected_outputs": ["课程创建成功"]
        },
        {
            "name": "学生选课",
            "input_sequence": [
                "1", "student1", "123456",  # 学生登录
                "1", "2", "1", "0"  # 浏览课程 -> 选课
            ],
            "expected_outputs": ["选课成功"]
        },
        {
            "name": "时间冲突检测",
            "input_sequence": [
                "1", "teacher1", "123456",  # 教师登录
                "2", "算法设计", "3", "2024-2025-1", "50", "周一1-2",  # 创建冲突课程
                "0", "0",
                "1", "student1", "123456",  # 学生登录
                "2", "2", "0", "0"  # 尝试选冲突课程
            ],
            "expected_outputs": ["时间冲突", "选课失败"]
        },
        {
            "name": "管理员用户管理",
            "input_sequence": [
                "1", "admin", "admin123",  # 管理员登录
                "3", "2", "3", "New Name",  # 修改用户姓名
                "0", "0"
            ],
            "expected_outputs": ["修改成功！"]
        },
        {
            "name": "报表导出",
            "input_sequence": [
                "1", "admin", "admin123",  # 管理员登录
                "5", "1", "0", "0"  # 导出报表
            ],
            "expected_outputs": ["已导出到"]
        }
    ]

    passed = 0
    failed = 0

    for test_case in test_cases:
        # 每次测试前清理数据库
        if not clean_database():
            print("清理数据库失败，停止测试")
            break

        # 编译
        if not compile_project():
            print("编译失败，停止测试")
            break

        # 运行测试
        if run_test_case(
            test_case["name"],
            test_case["input_sequence"],
            test_case["expected_outputs"]
        ):
            passed += 1
        else:
            failed += 1

    # 最终清理
    clean_database()

    print("\n" + "=" * 60)
    print(f"测试完成: 通过 {passed} 个，失败 {failed} 个")
    if failed == 0:
        print("✅ 所有测试通过！")
    else:
        print("❌ 部分测试失败")
    print("=" * 60)

if __name__ == "__main__":
    main()
