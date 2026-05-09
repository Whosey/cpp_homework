cmake_minimum_required(VERSION 3.16)

project(CourseStudyPlanner VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp

    models/User.h
    models/User.cpp
    models/Course.h
    models/Course.cpp
    models/BaseTask.h
    models/BaseTask.cpp
    models/HomeworkTask.h
    models/HomeworkTask.cpp
    models/SelfStudyTask.h
    models/SelfStudyTask.cpp
    models/ExamTask.h
    models/ExamTask.cpp
    models/Plan.h
    models/Plan.cpp
    models/Reminder.h
    models/Reminder.cpp

    managers/FileManager.h
    managers/FileManager.cpp
    managers/SystemManager.h
    managers/SystemManager.cpp

    pages/LoginPage.h
    pages/LoginPage.cpp
    pages/MainWindow.h
    pages/MainWindow.cpp
    pages/CoursePage.h
    pages/CoursePage.cpp
    pages/TaskPage.h
    pages/TaskPage.cpp
    pages/PlanPage.h
    pages/PlanPage.cpp
    pages/StatsPage.h
    pages/StatsPage.cpp
)

add_executable(CourseStudyPlanner ${PROJECT_SOURCES})

target_link_libraries(CourseStudyPlanner PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if (MSVC)
    target_compile_options(CourseStudyPlanner PRIVATE /utf-8)
else()
    target_compile_options(CourseStudyPlanner PRIVATE -finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

if (WIN32)
    set_target_properties(CourseStudyPlanner PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
