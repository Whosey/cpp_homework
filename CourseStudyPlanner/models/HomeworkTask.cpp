#include "HomeworkTask.h"

QString HomeworkTask::getType() const
{
    return "Homework";
}

QString HomeworkTask::getDisplayText() const
{
    return "[课程作业] " + BaseTask::getDisplayText();
}
