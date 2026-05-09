#ifndef SELFSTUDYTASK_H
#define SELFSTUDYTASK_H

#include "BaseTask.h"

class SelfStudyTask : public BaseTask
{
public:
    using BaseTask::BaseTask;

    QString getType() const override;
    QString getDisplayText() const override;
};

#endif
