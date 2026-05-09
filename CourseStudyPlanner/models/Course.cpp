#include "Course.h"

Course::Course()
{
}

Course::Course(const QString& courseId,
               const QString& ownerUserId,
               const QString& courseName,
               const QString& teacherName,
               const QString& note)
    : courseId(courseId),
      ownerUserId(ownerUserId),
      courseName(courseName),
      teacherName(teacherName),
      note(note)
{
}

QString Course::getCourseId() const { return courseId; }
QString Course::getOwnerUserId() const { return ownerUserId; }
QString Course::getCourseName() const { return courseName; }
QString Course::getTeacherName() const { return teacherName; }
QString Course::getNote() const { return note; }

void Course::setCourseId(const QString& courseId) { this->courseId = courseId; }
void Course::setOwnerUserId(const QString& ownerUserId) { this->ownerUserId = ownerUserId; }
void Course::setCourseName(const QString& courseName) { this->courseName = courseName; }
void Course::setTeacherName(const QString& teacherName) { this->teacherName = teacherName; }
void Course::setNote(const QString& note) { this->note = note; }

QJsonObject Course::toJson() const
{
    QJsonObject obj;
    obj["courseId"] = courseId;
    obj["ownerUserId"] = ownerUserId;
    obj["courseName"] = courseName;
    obj["teacherName"] = teacherName;
    obj["note"] = note;
    return obj;
}

Course Course::fromJson(const QJsonObject& obj)
{
    return Course(obj["courseId"].toString(),
                  obj["ownerUserId"].toString(),
                  obj["courseName"].toString(),
                  obj["teacherName"].toString(),
                  obj["note"].toString());
}
