#include "SelfStudyTask.h"

QString SelfStudyTask::getType() const
{
    return "SelfStudy";
}

QString SelfStudyTask::getDisplayText() const
{
    return "[自主学习] " + BaseTask::getDisplayText();
}
