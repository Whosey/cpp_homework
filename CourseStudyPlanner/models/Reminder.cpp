#include "Reminder.h"
#include "Plan.h"

QList<BaseTask*> Reminder::getDeadlineReminders(const QList<BaseTask*>& tasks, const QString& userId, int days)
{
    QList<BaseTask*> result;
    const QDate today = QDate::currentDate();
    const QDate endDate = today.addDays(days);
    for (BaseTask* task : tasks) {
        if (task && task->getOwnerUserId() == userId && !task->getIsCompleted()
            && task->getDeadline() >= today && task->getDeadline() <= endDate) {
            result.append(task);
        }
    }
    Plan::sortByDeadline(result);
    return result;
}

QList<BaseTask*> Reminder::getHighPriorityReminders(const QList<BaseTask*>& tasks, const QString& userId)
{
    QList<BaseTask*> result;
    for (BaseTask* task : tasks) {
        if (task && task->getOwnerUserId() == userId && !task->getIsCompleted()
            && task->getPriority() == 3) {
            result.append(task);
        }
    }
    Plan::sortByDeadline(result);
    return result;
}
