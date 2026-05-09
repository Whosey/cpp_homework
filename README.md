# CourseStudyPlanner 课程作业与学习规划管理系统

CourseStudyPlanner 是一个基于 C++17 和 Qt Widgets 开发的桌面端课程作业与学习规划管理系统。项目面向学生日常学习管理场景，提供账号登录、课程维护、任务记录、学习计划、截止提醒和任务统计等功能，帮助用户把课程、作业、自主学习和考试复习统一管理起来。

项目使用 CMake 构建，界面完全由 C++ 代码创建，不依赖 Qt Designer 的 `.ui` 文件。数据通过本地 JSON 文件保存，适合作为 C++ 面向对象程序设计、Qt 桌面应用开发、文件持久化和简单业务系统设计的课程项目。

## 项目功能

- 用户账号：支持注册、登录、退出登录和修改密码。
- 课程管理：支持添加、修改、删除课程，记录课程名称、任课教师和备注。
- 任务管理：支持添加、编辑、删除任务，并切换任务完成状态。
- 任务分类：任务分为课程作业、自主学习、考试复习三类。
- 任务属性：每个任务包含标题、描述、截止日期、优先级、所属课程和完成状态。
- 筛选排序：任务可按课程、类型、状态筛选，可按截止日期或优先级排序。
- 学习计划：自动生成今日任务和未来一周任务。
- 提醒功能：展示临近截止任务和高优先级任务。
- 数据统计：统计总任务数、已完成任务数、未完成任务数、本周完成率，以及每门课程的任务数量。
- 本地持久化：用户、课程、任务数据保存到 JSON 文件，关闭程序后再次打开仍可读取。

## 技术栈

- 编程语言：C++17
- 图形界面：Qt Widgets，兼容 Qt 5 / Qt 6
- 构建工具：CMake 3.16+
- 数据存储：本地 JSON 文件，使用 `QJsonDocument`、`QJsonObject`、`QJsonArray` 读写
- 主要模块：模型层、数据管理层、业务管理层、页面层

## 项目结构

```text
cpphomework/
├── README.md
├── .gitignore
└── CourseStudyPlanner/
    ├── CMakeLists.txt
    ├── README.md
    ├── main.cpp
    ├── data/
    │   ├── users.json
    │   ├── courses.json
    │   └── tasks.json
    ├── managers/
    │   ├── FileManager.h / FileManager.cpp
    │   └── SystemManager.h / SystemManager.cpp
    ├── models/
    │   ├── User.h / User.cpp
    │   ├── Course.h / Course.cpp
    │   ├── BaseTask.h / BaseTask.cpp
    │   ├── HomeworkTask.h / HomeworkTask.cpp
    │   ├── SelfStudyTask.h / SelfStudyTask.cpp
    │   ├── ExamTask.h / ExamTask.cpp
    │   ├── Plan.h / Plan.cpp
    │   └── Reminder.h / Reminder.cpp
    └── pages/
        ├── LoginPage.h / LoginPage.cpp
        ├── MainWindow.h / MainWindow.cpp
        ├── CoursePage.h / CoursePage.cpp
        ├── TaskPage.h / TaskPage.cpp
        ├── PlanPage.h / PlanPage.cpp
        └── StatsPage.h / StatsPage.cpp
```

## 模块说明

### models

模型层负责描述系统中的核心实体。

- `User`：用户账号信息，包含用户 ID、用户名和密码校验逻辑。
- `Course`：课程信息，包含课程 ID、所属用户、课程名称、教师姓名和备注。
- `BaseTask`：任务抽象基类，保存任务通用字段。
- `HomeworkTask`：课程作业任务。
- `SelfStudyTask`：自主学习任务。
- `ExamTask`：考试复习任务。
- `Plan`：根据截止日期和优先级生成今日计划、本周计划，并提供排序逻辑。
- `Reminder`：生成临近截止提醒和高优先级提醒。

### managers

管理层负责数据读写和业务逻辑。

- `FileManager`：统一负责 JSON 文件的创建、读取和写入。程序启动时会在可执行文件所在目录下创建 `data/users.json`、`data/courses.json`、`data/tasks.json`。
- `SystemManager`：负责用户登录注册、课程增删改、任务增删改、计划获取、提醒获取和统计计算。页面层不直接读写文件，而是通过 `SystemManager` 调用业务能力。

### pages

页面层负责 Qt Widgets 界面。

- `LoginPage`：登录与注册页面。
- `MainWindow`：主窗口，左侧为导航区，右侧为功能页面。
- `CoursePage`：课程管理页面。
- `TaskPage`：任务管理页面。
- `PlanPage`：学习计划和提醒页面。
- `StatsPage`：统计信息页面。

## 环境准备

运行项目需要先安装以下工具：

1. C++ 编译器
   - Windows 推荐 MinGW 或 MSVC。
   - 如果使用 Qt 官方安装器，可以在安装 Qt 时一并安装 MinGW。

2. Qt
   - 支持 Qt 5 或 Qt 6。
   - 需要安装 `Widgets` 模块。
   - Windows 示例路径可能类似：
     - `C:\Qt\6.6.3\mingw_64`
     - `C:\Qt\5.15.2\mingw81_64`

3. CMake
   - 版本要求：3.16 或更高。
   - 可以通过命令检查：

```powershell
cmake --version
```

4. Git
   - 用于克隆代码：

```powershell
git --version
```

## 获取代码

在合适的目录中执行：

```powershell
git clone https://github.com/Whosey/cpp_homework.git
cd cpp_homework\CourseStudyPlanner
```

如果你已经下载了压缩包，也可以直接解压后进入 `CourseStudyPlanner` 目录。

## 使用命令行构建和运行

以下命令以 Windows PowerShell 为例。

### 1. 进入项目目录

```powershell
cd E:\c++project\cpphomework\CourseStudyPlanner
```

如果是从 GitHub 克隆下来的项目，则进入：

```powershell
cd cpp_homework\CourseStudyPlanner
```

### 2. 配置 CMake

需要把 `CMAKE_PREFIX_PATH` 设置为你的 Qt 安装目录。示例：

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.6.3/mingw_64"
```

如果你使用的是 Qt 5，可以写成类似：

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/5.15.2/mingw81_64"
```

如果 CMake 提示找不到 Qt，请检查：

- Qt 路径是否写到了包含 `lib/cmake` 的 Qt kit 目录。
- Qt 版本是否安装了 Widgets 模块。
- 使用的编译器是否和 Qt kit 匹配。例如 MinGW 版 Qt 对应 MinGW 编译器，MSVC 版 Qt 对应 MSVC 编译器。

### 3. 编译项目

```powershell
cmake --build build
```

如果使用多配置生成器，例如 Visual Studio，可能需要指定配置：

```powershell
cmake --build build --config Release
```

### 4. 运行程序

MinGW 或单配置构建时，通常运行：

```powershell
.\build\CourseStudyPlanner.exe
```

Visual Studio 或多配置构建时，通常运行：

```powershell
.\build\Release\CourseStudyPlanner.exe
```

如果程序启动时报缺少 Qt 动态库，可以使用 Qt 自带的 `windeployqt` 部署依赖。示例：

```powershell
windeployqt .\build\CourseStudyPlanner.exe
```

多配置构建则可能是：

```powershell
windeployqt .\build\Release\CourseStudyPlanner.exe
```

## 使用 Qt Creator 运行

如果不想使用命令行，也可以用 Qt Creator 打开项目。

1. 启动 Qt Creator。
2. 点击 `File` -> `Open File or Project`。
3. 选择 `CourseStudyPlanner/CMakeLists.txt`。
4. 选择一个已安装的 Qt Kit，例如 Desktop Qt 6.x MinGW 或 Desktop Qt 5.x MinGW。
5. 等待 CMake 配置完成。
6. 点击左下角绿色运行按钮，或按 `Ctrl + R`。

如果配置失败，优先检查 Qt Kit 是否完整、编译器是否匹配、CMake 是否能找到 Qt。

## 数据文件说明

程序运行时会在可执行文件所在目录自动创建 `data` 文件夹，并维护三个 JSON 文件：

- `users.json`：保存用户账号。
- `courses.json`：保存课程信息。
- `tasks.json`：保存任务信息。

源码目录中的 `CourseStudyPlanner/data/` 提供了初始空 JSON 文件，方便展示项目结构。实际运行时，程序读写的是可执行文件目录下的 `data` 文件夹。例如：

```text
CourseStudyPlanner/build/data/users.json
CourseStudyPlanner/build/data/courses.json
CourseStudyPlanner/build/data/tasks.json
```

如果你使用 Visual Studio 多配置构建，数据文件可能位于：

```text
CourseStudyPlanner/build/Release/data/
```

账号、课程和任务都保存在本地文件中。更换构建目录或删除运行目录下的 `data` 文件夹后，程序会重新创建空数据文件。

## App 使用说明

### 1. 注册账号

启动程序后会进入登录/注册页面。

1. 输入用户名。
2. 输入密码。
3. 点击“注册”。
4. 注册成功后，使用同一组用户名和密码登录。

用户名不能为空，也不能和已有用户名重复；密码不能为空。

### 2. 登录系统

1. 输入已注册的用户名。
2. 输入密码。
3. 点击“登录”。
4. 登录成功后进入主窗口。

登录后左侧是导航栏，右侧显示当前功能页面。

### 3. 课程管理

进入“课程管理”页面后，可以维护当前用户自己的课程。

- 点击“添加课程”可以新增课程。
- 选择表格中的一门课程后，点击“修改课程”可以编辑课程名称、教师姓名和备注。
- 选择表格中的一门课程后，点击“删除课程”可以删除课程。

删除课程时，已经关联到该课程的任务不会被删除，但会取消课程关联，之后会显示为未关联课程。

### 4. 任务管理

进入“任务管理”页面后，可以管理学习任务。

新增任务时需要填写：

- 任务标题：必填。
- 任务描述：记录任务内容。
- 截止日期：用于排序、计划和提醒。
- 优先级：Low、Medium、High。
- 任务类型：课程作业、自主学习、考试复习。
- 所属课程：可以选择已有课程，也可以保持未关联课程。

任务页面支持：

- 添加任务。
- 编辑任务。
- 删除任务。
- 使用“完成 / 未完成”按钮切换状态。
- 按课程筛选任务。
- 按任务类型筛选任务。
- 按完成状态筛选任务。
- 按截止日期排序。
- 按优先级排序。

### 5. 学习计划

进入“学习计划”页面后，系统会自动汇总未完成任务：

- 今日任务：截止日期为今天的未完成任务，按优先级排序。
- 本周任务：从今天起未来 7 天内截止的未完成任务，按截止日期排序。
- 截止提醒：未来 2 天内截止的未完成任务。
- 高优先级提醒：优先级为 High 的未完成任务。

这个页面适合每天打开后快速查看当天最需要处理的事项。

### 6. 统计信息

进入“统计信息”页面后，可以查看当前用户的任务统计：

- 总任务数量。
- 已完成任务数量。
- 未完成任务数量。
- 本周任务完成情况。
- 本周完成率进度条。
- 每门课程对应的任务数量。

统计信息会根据任务状态、截止日期和课程关联自动更新。

### 7. 修改密码和退出登录

在主窗口左侧导航栏中：

- 点击“修改密码”可以输入原密码和新密码完成密码修改。
- 点击“退出登录”会返回登录页面。

## 建议测试流程

可以按下面的顺序快速验证系统主要功能：

1. 启动程序，注册账号，例如 `test` / `123456`。
2. 使用该账号登录。
3. 在“课程管理”中添加课程，例如 `C++ Programming`。
4. 在“任务管理”中添加三类任务：课程作业、自主学习、考试复习。
5. 为任务设置不同截止日期和不同优先级。
6. 使用课程、类型、状态筛选任务。
7. 分别按截止日期和优先级排序。
8. 选择一个任务，点击“完成 / 未完成”切换状态。
9. 进入“学习计划”，查看今日任务、本周任务和提醒列表。
10. 进入“统计信息”，查看完成率和课程任务数量。
11. 退出程序后重新启动，确认账号、课程和任务仍然存在。

## 常见问题

### CMake 找不到 Qt

请检查 `CMAKE_PREFIX_PATH` 是否指向正确的 Qt kit 目录，而不是 Qt 安装根目录。正确路径通常类似：

```text
C:/Qt/6.6.3/mingw_64
```

### 程序启动后提示缺少 DLL

这是 Windows 上 Qt 动态库没有被找到。可以：

- 从 Qt Creator 中运行。
- 将 Qt 的 `bin` 目录加入 `PATH`。
- 对生成的 exe 执行 `windeployqt`。

### 数据保存在哪里

数据保存在可执行文件所在目录的 `data` 文件夹中，不一定是源码目录的 `CourseStudyPlanner/data`。如果你换了构建目录，看到的数据也可能不同。

### 为什么 GitHub 上没有上传 zip 文件

根目录中的课程打包 zip 属于生成文件，已经通过 `.gitignore` 排除。Git 仓库只保留源码、CMake 配置、README 和必要的初始 JSON 文件。

## 后续可扩展方向

- 增加任务搜索功能。
- 增加日历视图。
- 增加任务导出功能。
- 增加密码加密存储。
- 增加数据备份和恢复功能。
- 增加更细粒度的统计图表。
- 将 JSON 存储替换为 SQLite。
