#include "BaseTask.h"

BaseTask::BaseTask()
    : priority(2), isCompleted(false)
{
}

BaseTask::BaseTask(const QString& taskId,
                   const QString& ownerUserId,
                   const QString& title,
                   const QString& description,
                   const QDate& deadline,
                   int priority,
                   bool isCompleted,
                   const QString& relatedCourseId)
    : taskId(taskId),
      ownerUserId(ownerUserId),
      title(title),
      description(description),
      deadline(deadline),
      priority(priority),
      isCompleted(isCompleted),
      relatedCourseId(relatedCourseId)
{
}

BaseTask::~BaseTask()
{
}

QString BaseTask::getTaskId() const { return taskId; }
QString BaseTask::getOwnerUserId() const { return ownerUserId; }
QString BaseTask::getTitle() const { return title; }
QString BaseTask::getDescription() const { return description; }
QDate BaseTask::getDeadline() const { return deadline; }
int BaseTask::getPriority() const { return priority; }
bool BaseTask::getIsCompleted() const { return isCompleted; }
QString BaseTask::getRelatedCourseId() const { return relatedCourseId; }

void BaseTask::setTaskId(const QString& taskId) { this->taskId = taskId; }
void BaseTask::setOwnerUserId(const QString& ownerUserId) { this->ownerUserId = ownerUserId; }
void BaseTask::setTitle(const QString& title) { this->title = title; }
void BaseTask::setDescription(const QString& description) { this->description = description; }
void BaseTask::setDeadline(const QDate& deadline) { this->deadline = deadline; }
void BaseTask::setPriority(int priority) { this->priority = priority; }
void BaseTask::setIsCompleted(bool isCompleted) { this->isCompleted = isCompleted; }
void BaseTask::setRelatedCourseId(const QString& relatedCourseId) { this->relatedCourseId = relatedCourseId; }

void BaseTask::markCompleted()
{
    isCompleted = true;
}

void BaseTask::markUncompleted()
{
    isCompleted = false;
}

QString BaseTask::getDisplayText() const
{
    return title + " - 截止：" + deadline.toString("yyyy-MM-dd");
}

QJsonObject BaseTask::toJson() const
{
    QJsonObject obj;
    obj["taskId"] = taskId;
    obj["ownerUserId"] = ownerUserId;
    obj["taskType"] = getType();
    obj["title"] = title;
    obj["description"] = description;
    obj["deadline"] = deadline.toString("yyyy-MM-dd");
    obj["priority"] = priority;
    obj["isCompleted"] = isCompleted;
    obj["relatedCourseId"] = relatedCourseId;
    return obj;
}
