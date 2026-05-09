#include "FileManager.h"

#include "../models/ExamTask.h"
#include "../models/HomeworkTask.h"
#include "../models/SelfStudyTask.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QStringList>

FileManager::FileManager()
{
    dataDirPath = QCoreApplication::applicationDirPath() + "/data";
    usersFilePath = dataDirPath + "/users.json";
    coursesFilePath = dataDirPath + "/courses.json";
    tasksFilePath = dataDirPath + "/tasks.json";
    ensureDataFiles();
}

void FileManager::ensureDataFiles()
{
    QDir dir(dataDirPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    const QStringList files = { usersFilePath, coursesFilePath, tasksFilePath };
    for (const QString& filePath : files) {
        if (!QFile::exists(filePath)) {
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                file.write("[]");
                file.close();
            }
        }
    }
}

QJsonArray FileManager::readJsonArray(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开 JSON 文件：" << filePath;
        return {};
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    file.close();

    if (error.error != QJsonParseError::NoError || !doc.isArray()) {
        qWarning() << "JSON 读取失败：" << filePath << error.errorString();
        return {};
    }

    return doc.array();
}

bool FileManager::writeJsonArray(const QString& filePath, const QJsonArray& array)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "无法写入 JSON 文件：" << filePath;
        return false;
    }

    QJsonDocument doc(array);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

QList<User> FileManager::loadUsers()
{
    QList<User> users;
    const QJsonArray array = readJsonArray(usersFilePath);
    for (const QJsonValue& value : array) {
        if (value.isObject()) {
            users.append(User::fromJson(value.toObject()));
        }
    }
    return users;
}

bool FileManager::saveUsers(const QList<User>& users)
{
    QJsonArray array;
    for (const User& user : users) {
        array.append(user.toJson());
    }
    return writeJsonArray(usersFilePath, array);
}

QList<Course> FileManager::loadCourses()
{
    QList<Course> courses;
    const QJsonArray array = readJsonArray(coursesFilePath);
    for (const QJsonValue& value : array) {
        if (value.isObject()) {
            courses.append(Course::fromJson(value.toObject()));
        }
    }
    return courses;
}

bool FileManager::saveCourses(const QList<Course>& courses)
{
    QJsonArray array;
    for (const Course& course : courses) {
        array.append(course.toJson());
    }
    return writeJsonArray(coursesFilePath, array);
}

BaseTask* FileManager::createTaskFromJson(const QJsonObject& obj)
{
    const QString type = obj["taskType"].toString();
    const QString taskId = obj["taskId"].toString();
    const QString ownerUserId = obj["ownerUserId"].toString();
    const QString title = obj["title"].toString();
    const QString description = obj["description"].toString();
    const QDate deadline = QDate::fromString(obj["deadline"].toString(), "yyyy-MM-dd");
    const int priority = obj["priority"].toInt(2);
    const bool isCompleted = obj["isCompleted"].toBool(false);
    const QString relatedCourseId = obj["relatedCourseId"].toString();

    if (type == "Homework") {
        return new HomeworkTask(taskId, ownerUserId, title, description, deadline, priority, isCompleted, relatedCourseId);
    }
    if (type == "SelfStudy") {
        return new SelfStudyTask(taskId, ownerUserId, title, description, deadline, priority, isCompleted, relatedCourseId);
    }
    if (type == "Exam") {
        return new ExamTask(taskId, ownerUserId, title, description, deadline, priority, isCompleted, relatedCourseId);
    }

    qWarning() << "未知任务类型，已忽略：" << type;
    return nullptr;
}

QList<BaseTask*> FileManager::loadTasks()
{
    QList<BaseTask*> tasks;
    const QJsonArray array = readJsonArray(tasksFilePath);
    for (const QJsonValue& value : array) {
        if (value.isObject()) {
            BaseTask* task = createTaskFromJson(value.toObject());
            if (task) {
                tasks.append(task);
            }
        }
    }
    return tasks;
}

bool FileManager::saveTasks(const QList<BaseTask*>& tasks)
{
    QJsonArray array;
    for (BaseTask* task : tasks) {
        if (task) {
            array.append(task->toJson());
        }
    }
    return writeJsonArray(tasksFilePath, array);
}
