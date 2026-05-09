#ifndef BASETASK_H
#define BASETASK_H

#include <QDate>
#include <QJsonObject>
#include <QString>

class BaseTask
{
public:
    BaseTask();
    BaseTask(const QString& taskId,
             const QString& ownerUserId,
             const QString& title,
             const QString& description,
             const QDate& deadline,
             int priority,
             bool isCompleted,
             const QString& relatedCourseId);
    virtual ~BaseTask();

    QString getTaskId() const;
    QString getOwnerUserId() const;
    QString getTitle() const;
    QString getDescription() const;
    QDate getDeadline() const;
    int getPriority() const;
    bool getIsCompleted() const;
    QString getRelatedCourseId() const;

    void setTaskId(const QString& taskId);
    void setOwnerUserId(const QString& ownerUserId);
    void setTitle(const QString& title);
    void setDescription(const QString& description);
    void setDeadline(const QDate& deadline);
    void setPriority(int priority);
    void setIsCompleted(bool isCompleted);
    void setRelatedCourseId(const QString& relatedCourseId);

    void markCompleted();
    void markUncompleted();

    virtual QString getType() const = 0;
    virtual QString getDisplayText() const;
    virtual QJsonObject toJson() const;

protected:
    QString taskId;
    QString ownerUserId;
    QString title;
    QString description;
    QDate deadline;
    int priority;
    bool isCompleted;
    QString relatedCourseId;
};

#endif
