#ifndef COURSE_H
#define COURSE_H

#include <QJsonObject>
#include <QString>

class Course
{
public:
    Course();
    Course(const QString& courseId,
           const QString& ownerUserId,
           const QString& courseName,
           const QString& teacherName,
           const QString& note);

    QString getCourseId() const;
    QString getOwnerUserId() const;
    QString getCourseName() const;
    QString getTeacherName() const;
    QString getNote() const;

    void setCourseId(const QString& courseId);
    void setOwnerUserId(const QString& ownerUserId);
    void setCourseName(const QString& courseName);
    void setTeacherName(const QString& teacherName);
    void setNote(const QString& note);

    QJsonObject toJson() const;
    static Course fromJson(const QJsonObject& obj);

private:
    QString courseId;
    QString ownerUserId;
    QString courseName;
    QString teacherName;
    QString note;
};

#endif
