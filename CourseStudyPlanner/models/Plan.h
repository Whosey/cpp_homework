#ifndef PLAN_H
#define PLAN_H

#include "BaseTask.h"

#include <QList>
#include <QString>

class Plan
{
public:
    QList<BaseTask*> getTodayTasks(const QList<BaseTask*>& tasks, const QString& userId);
    QList<BaseTask*> getWeekTasks(const QList<BaseTask*>& tasks, const QString& userId);

    static void sortByPriority(QList<BaseTask*>& tasks);
    static void sortByDeadline(QList<BaseTask*>& tasks);
};

#endif
