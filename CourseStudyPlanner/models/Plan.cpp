#include "Plan.h"

#include <algorithm>

QList<BaseTask*> Plan::getTodayTasks(const QList<BaseTask*>& tasks, const QString& userId)
{
    QList<BaseTask*> result;
    const QDate today = QDate::currentDate();
    for (BaseTask* task : tasks) {
        if (task && task->getOwnerUserId() == userId && !task->getIsCompleted()
            && task->getDeadline() == today) {
            result.append(task);
        }
    }
    sortByPriority(result);
    return result;
}

QList<BaseTask*> Plan::getWeekTasks(const QList<BaseTask*>& tasks, const QString& userId)
{
    QList<BaseTask*> result;
    const QDate today = QDate::currentDate();
    const QDate weekEnd = today.addDays(6);
    for (BaseTask* task : tasks) {
        if (task && task->getOwnerUserId() == userId && !task->getIsCompleted()
            && task->getDeadline() >= today && task->getDeadline() <= weekEnd) {
            result.append(task);
        }
    }
    sortByDeadline(result);
    return result;
}

void Plan::sortByPriority(QList<BaseTask*>& tasks)
{
    std::sort(tasks.begin(), tasks.end(), [](BaseTask* a, BaseTask* b) {
        if (a->getPriority() == b->getPriority()) {
            return a->getDeadline() < b->getDeadline();
        }
        return a->getPriority() > b->getPriority();
    });
}

void Plan::sortByDeadline(QList<BaseTask*>& tasks)
{
    std::sort(tasks.begin(), tasks.end(), [](BaseTask* a, BaseTask* b) {
        if (a->getDeadline() == b->getDeadline()) {
            return a->getPriority() > b->getPriority();
        }
        return a->getDeadline() < b->getDeadline();
    });
}
