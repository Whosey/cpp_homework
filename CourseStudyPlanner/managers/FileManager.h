#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "../models/BaseTask.h"
#include "../models/Course.h"
#include "../models/User.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QString>

class FileManager
{
public:
    FileManager();

    QList<User> loadUsers();
    bool saveUsers(const QList<User>& users);

    QList<Course> loadCourses();
    bool saveCourses(const QList<Course>& courses);

    QList<BaseTask*> loadTasks();
    bool saveTasks(const QList<BaseTask*>& tasks);

private:
    QString dataDirPath;
    QString usersFilePath;
    QString coursesFilePath;
    QString tasksFilePath;

    void ensureDataFiles();
    QJsonArray readJsonArray(const QString& filePath);
    bool writeJsonArray(const QString& filePath, const QJsonArray& array);
    BaseTask* createTaskFromJson(const QJsonObject& obj);
};

#endif
