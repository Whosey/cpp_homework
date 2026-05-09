#ifndef HOMEWORKTASK_H
#define HOMEWORKTASK_H

#include "BaseTask.h"

class HomeworkTask : public BaseTask
{
public:
    using BaseTask::BaseTask;

    QString getType() const override;
    QString getDisplayText() const override;
};

#endif
