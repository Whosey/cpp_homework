#include "SystemManager.h"

#include "../models/ExamTask.h"
#include "../models/HomeworkTask.h"
#include "../models/SelfStudyTask.h"

#include <QDateTime>
#include <QtAlgorithms>

SystemManager::SystemManager()
    : currentUser(nullptr)
{
    loadAll();
}

SystemManager::~SystemManager()
{
    saveAll();
    qDeleteAll(tasks);
    tasks.clear();
}

void SystemManager::loadAll()
{
    users = fileManager.loadUsers();
    courses = fileManager.loadCourses();
    qDeleteAll(tasks);
    tasks = fileManager.loadTasks();
    currentUser = nullptr;
}

void SystemManager::saveAll()
{
    fileManager.saveUsers(users);
    fileManager.saveCourses(courses);
    fileManager.saveTasks(tasks);
}

QString SystemManager::generateId(const QString& prefix) const
{
    return prefix + "_" + QString::number(QDateTime::currentMSecsSinceEpoch());
}

bool SystemManager::registerUser(const QString& username, const QString& password, QString& errorMessage)
{
    const QString cleanUsername = username.trimmed();
    if (cleanUsername.isEmpty()) {
        errorMessage = "用户名不能为空。";
        return false;
    }
    if (password.isEmpty()) {
        errorMessage = "密码不能为空。";
        return false;
    }
    for (const User& user : users) {
        if (user.getUsername() == cleanUsername) {
            errorMessage = "用户名已存在。";
            return false;
        }
    }

    users.append(User(generateId("u"), cleanUsername, password));
    saveAll();
    return true;
}

bool SystemManager::login(const QString& username, const QString& password, QString& errorMessage)
{
    const QString cleanUsername = username.trimmed();
    for (User& user : users) {
        if (user.getUsername() == cleanUsername) {
            if (!user.checkPassword(password)) {
                errorMessage = "密码错误。";
                return false;
            }
            currentUser = &user;
            return true;
        }
    }

    errorMessage = "用户名不存在。";
    return false;
}

void SystemManager::logout()
{
    currentUser = nullptr;
}

bool SystemManager::changePassword(const QString& oldPassword, const QString& newPassword, QString& errorMessage)
{
    if (!currentUser) {
        errorMessage = "请先登录。";
        return false;
    }
    if (!currentUser->checkPassword(oldPassword)) {
        errorMessage = "原密码错误。";
        return false;
    }
    if (newPassword.isEmpty()) {
        errorMessage = "新密码不能为空。";
        return false;
    }
    currentUser->changePassword(newPassword);
    saveAll();
    return true;
}

bool SystemManager::hasCurrentUser() const
{
    return currentUser != nullptr;
}

QString SystemManager::getCurrentUserId() const
{
    return currentUser ? currentUser->getUserId() : QString();
}

QString SystemManager::getCurrentUsername() const
{
    return currentUser ? currentUser->getUsername() : QString();
}

QList<Course> SystemManager::getCurrentUserCourses() const
{
    QList<Course> result;
    const QString userId = getCurrentUserId();
    for (const Course& course : courses) {
        if (course.getOwnerUserId() == userId) {
            result.append(course);
        }
    }
    return result;
}

QList<BaseTask*> SystemManager::getCurrentUserTasks() const
{
    QList<BaseTask*> result;
    const QString userId = getCurrentUserId();
    for (BaseTask* task : tasks) {
        if (task && task->getOwnerUserId() == userId) {
            result.append(task);
        }
    }
    return result;
}

bool SystemManager::addCourse(const QString& courseName, const QString& teacherName, const QString& note, QString& errorMessage)
{
    if (!currentUser) {
        errorMessage = "请先登录。";
        return false;
    }
    if (courseName.trimmed().isEmpty()) {
        errorMessage = "课程名不能为空。";
        return false;
    }

    courses.append(Course(generateId("c"), currentUser->getUserId(), courseName.trimmed(), teacherName.trimmed(), note.trimmed()));
    saveAll();
    return true;
}

bool SystemManager::updateCourse(const QString& courseId, const QString& courseName, const QString& teacherName, const QString& note, QString& errorMessage)
{
    if (courseName.trimmed().isEmpty()) {
        errorMessage = "课程名不能为空。";
        return false;
    }
    for (Course& course : courses) {
        if (course.getCourseId() == courseId && course.getOwnerUserId() == getCurrentUserId()) {
            course.setCourseName(courseName.trimmed());
            course.setTeacherName(teacherName.trimmed());
            course.setNote(note.trimmed());
            saveAll();
            return true;
        }
    }
    errorMessage = "未找到要修改的课程。";
    return false;
}

bool SystemManager::deleteCourse(const QString& courseId, QString& errorMessage)
{
    for (int i = 0; i < courses.size(); ++i) {
        if (courses[i].getCourseId() == courseId && courses[i].getOwnerUserId() == getCurrentUserId()) {
            courses.removeAt(i);
            // 删除课程后，关联任务保留，但取消课程关联，避免悬空课程名。
            for (BaseTask* task : tasks) {
                if (task && task->getRelatedCourseId() == courseId) {
                    task->setRelatedCourseId("");
                }
            }
            saveAll();
            return true;
        }
    }
    errorMessage = "未找到要删除的课程。";
    return false;
}

BaseTask* SystemManager::createTask(const QString& taskId,
                                    const QString& ownerUserId,
                                    const QString& title,
                                    const QString& description,
                                    const QDate& deadline,
                                    int priority,
                                    bool isCompleted,
                                    const QString& relatedCourseId,
                                    const QString& taskType) const
{
    if (taskType == "Homework") {
        return new HomeworkTask(taskId, ownerUserId, title, description, deadline, priority, isCompleted, relatedCourseId);
    }
    if (taskType == "SelfStudy") {
        return new SelfStudyTask(taskId, ownerUserId, title, description, deadline, priority, isCompleted, relatedCourseId);
    }
    return new ExamTask(taskId, ownerUserId, title, description, deadline, priority, isCompleted, relatedCourseId);
}

bool SystemManager::addTask(const QString& title,
                            const QString& description,
                            const QDate& deadline,
                            int priority,
                            const QString& relatedCourseId,
                            const QString& taskType,
                            QString& errorMessage)
{
    if (!currentUser) {
        errorMessage = "请先登录。";
        return false;
    }
    if (title.trimmed().isEmpty()) {
        errorMessage = "任务标题不能为空。";
        return false;
    }
    if (!deadline.isValid()) {
        errorMessage = "截止日期无效。";
        return false;
    }

    tasks.append(createTask(generateId("t"), currentUser->getUserId(), title.trimmed(), description.trimmed(),
                            deadline, priority, false, relatedCourseId, taskType));
    saveAll();
    return true;
}

bool SystemManager::updateTask(const QString& taskId,
                               const QString& title,
                               const QString& description,
                               const QDate& deadline,
                               int priority,
                               bool isCompleted,
                               const QString& relatedCourseId,
                               const QString& taskType,
                               QString& errorMessage)
{
    if (title.trimmed().isEmpty()) {
        errorMessage = "任务标题不能为空。";
        return false;
    }
    if (!deadline.isValid()) {
        errorMessage = "截止日期无效。";
        return false;
    }

    for (int i = 0; i < tasks.size(); ++i) {
        BaseTask* oldTask = tasks[i];
        if (oldTask && oldTask->getTaskId() == taskId && oldTask->getOwnerUserId() == getCurrentUserId()) {
            BaseTask* newTask = createTask(taskId, getCurrentUserId(), title.trimmed(), description.trimmed(),
                                           deadline, priority, isCompleted, relatedCourseId, taskType);
            delete oldTask;
            tasks[i] = newTask;
            saveAll();
            return true;
        }
    }

    errorMessage = "未找到要修改的任务。";
    return false;
}

bool SystemManager::deleteTask(const QString& taskId, QString& errorMessage)
{
    for (int i = 0; i < tasks.size(); ++i) {
        BaseTask* task = tasks[i];
        if (task && task->getTaskId() == taskId && task->getOwnerUserId() == getCurrentUserId()) {
            delete task;
            tasks.removeAt(i);
            saveAll();
            return true;
        }
    }
    errorMessage = "未找到要删除的任务。";
    return false;
}

bool SystemManager::toggleTaskCompleted(const QString& taskId, QString& errorMessage)
{
    for (BaseTask* task : tasks) {
        if (task && task->getTaskId() == taskId && task->getOwnerUserId() == getCurrentUserId()) {
            task->setIsCompleted(!task->getIsCompleted());
            saveAll();
            return true;
        }
    }
    errorMessage = "未找到任务。";
    return false;
}

QList<BaseTask*> SystemManager::getTodayPlan()
{
    return plan.getTodayTasks(tasks, getCurrentUserId());
}

QList<BaseTask*> SystemManager::getWeekPlan()
{
    return plan.getWeekTasks(tasks, getCurrentUserId());
}

QList<BaseTask*> SystemManager::getDeadlineReminders()
{
    return reminder.getDeadlineReminders(tasks, getCurrentUserId(), 2);
}

QList<BaseTask*> SystemManager::getHighPriorityReminders()
{
    return reminder.getHighPriorityReminders(tasks, getCurrentUserId());
}

QVariantMap SystemManager::getStatistics()
{
    QVariantMap stats;
    int totalTasks = 0;
    int completedTasks = 0;
    int weekTotalTasks = 0;
    int weekCompletedTasks = 0;
    QVariantMap courseTaskCount;

    const QString userId = getCurrentUserId();
    const QDate today = QDate::currentDate();
    const QDate weekEnd = today.addDays(6);

    for (BaseTask* task : tasks) {
        if (!task || task->getOwnerUserId() != userId) {
            continue;
        }

        ++totalTasks;
        if (task->getIsCompleted()) {
            ++completedTasks;
        }

        if (task->getDeadline() >= today && task->getDeadline() <= weekEnd) {
            ++weekTotalTasks;
            if (task->getIsCompleted()) {
                ++weekCompletedTasks;
            }
        }

        const QString courseName = getCourseNameById(task->getRelatedCourseId());
        const QString displayName = courseName.isEmpty() ? "未关联课程" : courseName;
        courseTaskCount[displayName] = courseTaskCount.value(displayName).toInt() + 1;
    }

    const int completionRate = weekTotalTasks == 0 ? 0 : static_cast<int>(weekCompletedTasks * 100.0 / weekTotalTasks);
    stats["totalTasks"] = totalTasks;
    stats["completedTasks"] = completedTasks;
    stats["unfinishedTasks"] = totalTasks - completedTasks;
    stats["weekTotalTasks"] = weekTotalTasks;
    stats["weekCompletedTasks"] = weekCompletedTasks;
    stats["completionRate"] = completionRate;
    stats["courseTaskCount"] = courseTaskCount;
    return stats;
}

QString SystemManager::getCourseNameById(const QString& courseId) const
{
    if (courseId.isEmpty()) {
        return "";
    }
    for (const Course& course : courses) {
        if (course.getCourseId() == courseId && course.getOwnerUserId() == getCurrentUserId()) {
            return course.getCourseName();
        }
    }
    return "";
}
bool SystemManager::addLearningLogToTask(const QString& taskId, const QString& log, QString& errorMessage)
{
    for (BaseTask* task : tasks) {
        if (task && task->getTaskId() == taskId && task->getOwnerUserId() == getCurrentUserId()) {
            // 自动加上今天的日期，例如 "2026-05-24: 今天搞懂了曲线积分"
            QString datedLog = QDate::currentDate().toString("yyyy-MM-dd") + ": " + log.trimmed();
            task->addLearningLog(datedLog);
            saveAll();
            return true;
        }
    }
    errorMessage = "未找到任务。";
    return false;
}