#ifndef EXAMTASK_H
#define EXAMTASK_H

#include "BaseTask.h"

class ExamTask : public BaseTask
{
public:
    using BaseTask::BaseTask;

    QString getType() const override;
    QString getDisplayText() const override;
};

#endif
