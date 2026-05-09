#ifndef REMINDER_H
#define REMINDER_H

#include "BaseTask.h"

#include <QList>
#include <QString>

class Reminder
{
public:
    QList<BaseTask*> getDeadlineReminders(const QList<BaseTask*>& tasks, const QString& userId, int days = 2);
    QList<BaseTask*> getHighPriorityReminders(const QList<BaseTask*>& tasks, const QString& userId);
};

#endif
