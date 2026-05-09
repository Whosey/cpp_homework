#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "../models/BaseTask.h"
#include "../models/Course.h"
#include "../models/Plan.h"
#include "../models/Reminder.h"
#include "../models/User.h"
#include "FileManager.h"

#include <QVariantMap>

class SystemManager
{
public:
    SystemManager();
    ~SystemManager();

    bool registerUser(const QString& username, const QString& password, QString& errorMessage);
    bool login(const QString& username, const QString& password, QString& errorMessage);
    void logout();
    bool changePassword(const QString& oldPassword, const QString& newPassword, QString& errorMessage);

    bool hasCurrentUser() const;
    QString getCurrentUserId() const;
    QString getCurrentUsername() const;

    QList<Course> getCurrentUserCourses() const;
    QList<BaseTask*> getCurrentUserTasks() const;

    bool addCourse(const QString& courseName, const QString& teacherName, const QString& note, QString& errorMessage);
    bool updateCourse(const QString& courseId, const QString& courseName, const QString& teacherName, const QString& note, QString& errorMessage);
    bool deleteCourse(const QString& courseId, QString& errorMessage);

    bool addTask(const QString& title,
                 const QString& description,
                 const QDate& deadline,
                 int priority,
                 const QString& relatedCourseId,
                 const QString& taskType,
                 QString& errorMessage);
    bool updateTask(const QString& taskId,
                    const QString& title,
                    const QString& description,
                    const QDate& deadline,
                    int priority,
                    bool isCompleted,
                    const QString& relatedCourseId,
                    const QString& taskType,
                    QString& errorMessage);
    bool deleteTask(const QString& taskId, QString& errorMessage);
    bool toggleTaskCompleted(const QString& taskId, QString& errorMessage);

    QList<BaseTask*> getTodayPlan();
    QList<BaseTask*> getWeekPlan();
    QList<BaseTask*> getDeadlineReminders();
    QList<BaseTask*> getHighPriorityReminders();
    QVariantMap getStatistics();

    QString getCourseNameById(const QString& courseId) const;

    void loadAll();
    void saveAll();

private:
    QList<User> users;
    QList<Course> courses;
    QList<BaseTask*> tasks;
    User* currentUser;
    FileManager fileManager;
    Plan plan;
    Reminder reminder;

    QString generateId(const QString& prefix) const;
    BaseTask* createTask(const QString& taskId,
                         const QString& ownerUserId,
                         const QString& title,
                         const QString& description,
                         const QDate& deadline,
                         int priority,
                         bool isCompleted,
                         const QString& relatedCourseId,
                         const QString& taskType) const;
};

#endif
