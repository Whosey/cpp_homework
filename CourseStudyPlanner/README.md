# Course Assignment and Study Planning Management System

基于 C++17 与 Qt Widgets 的课程作业与学习规划管理系统。项目使用 CMake 构建，界面全部由 C++ 代码创建，不依赖 `.ui` 文件。

## 构建运行

1. 安装 Qt 5 或 Qt 6，并确保 CMake 能找到 Qt。
2. 在项目目录执行：

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="你的Qt安装路径"
cmake --build build
```

示例 Qt 路径：

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.6.3/mingw_64"
cmake --build build
```

3. 运行生成的 `CourseStudyPlanner.exe`。

程序启动后会在可执行文件所在目录自动创建 `data/users.json`、`data/courses.json`、`data/tasks.json`。源码目录下也提供了初始空 JSON 文件，便于课程设计展示。

## 功能测试流程

1. 启动程序，注册账号 `test` / `123456`。
2. 使用该账号登录，进入主界面。
3. 在“课程管理”中添加课程，例如 `C++ Programming`。
4. 在“任务管理”中添加课程作业、自主学习、考试复习三类任务。
5. 使用课程、类型、状态筛选任务，并尝试按优先级或截止日期排序。
6. 选择任务，点击“完成 / 未完成”切换状态。
7. 进入“学习计划”，查看今日任务、本周任务和截止提醒。
8. 进入“统计信息”，查看完成数量、本周完成率和每门课程任务数量。
9. 退出程序后重新启动，确认账号、课程、任务仍可读取。

## 课程设计要点

- `BaseTask` 是抽象基类，`HomeworkTask`、`SelfStudyTask`、`ExamTask` 继承它并重写虚函数。
- `SystemManager` 负责业务逻辑，UI 页面不直接读写文件。
- `FileManager` 统一使用 `QJsonDocument`、`QJsonObject`、`QJsonArray` 读写本地 JSON。
- `Plan` 与 `Reminder` 独立封装计划生成和提醒逻辑。
