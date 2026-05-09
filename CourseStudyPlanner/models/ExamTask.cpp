#include "ExamTask.h"

QString ExamTask::getType() const
{
    return "Exam";
}

QString ExamTask::getDisplayText() const
{
    return "[考试复习] " + BaseTask::getDisplayText();
}
