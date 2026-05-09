#include "PlanPage.h"

#include <QAbstractItemView>
#include <QGroupBox>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

namespace {
QString priorityText(int priority)
{
    if (priority == 3) {
        return "High";
    }
    if (priority == 2) {
        return "Medium";
    }
    return "Low";
}

QString typeText(const QString& type)
{
    if (type == "Homework") {
        return "课程作业";
    }
    if (type == "SelfStudy") {
        return "自主学习";
    }
    return "考试复习";
}
}

PlanPage::PlanPage(SystemManager* manager, QWidget* parent)
    : QWidget(parent), manager(manager)
{
    QLabel* titleLabel = new QLabel("学习计划");
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");

    todayTable = new QTableWidget;
    weekTable = new QTableWidget;
    reminderList = new QListWidget;

    for (QTableWidget* table : { todayTable, weekTable }) {
        table->setColumnCount(5);
        table->setHorizontalHeaderLabels({ "标题", "类型", "课程", "截止日期", "优先级" });
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->horizontalHeader()->setStretchLastSection(true);
    }

    QGroupBox* todayBox = new QGroupBox("今日任务");
    QVBoxLayout* todayLayout = new QVBoxLayout(todayBox);
    todayLayout->addWidget(todayTable);

    QGroupBox* weekBox = new QGroupBox("本周任务");
    QVBoxLayout* weekLayout = new QVBoxLayout(weekBox);
    weekLayout->addWidget(weekTable);

    QGroupBox* reminderBox = new QGroupBox("截止提醒 / 高优先级提醒");
    QVBoxLayout* reminderLayout = new QVBoxLayout(reminderBox);
    reminderLayout->addWidget(reminderList);

    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->addWidget(todayBox, 1);
    topLayout->addWidget(reminderBox, 1);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addLayout(topLayout, 1);
    layout->addWidget(weekBox, 1);
}

void PlanPage::fillTaskTable(QTableWidget* table, const QList<BaseTask*>& tasks)
{
    table->setRowCount(tasks.size());
    for (int row = 0; row < tasks.size(); ++row) {
        BaseTask* task = tasks[row];
        table->setItem(row, 0, new QTableWidgetItem(task->getTitle()));
        table->setItem(row, 1, new QTableWidgetItem(typeText(task->getType())));
        table->setItem(row, 2, new QTableWidgetItem(manager->getCourseNameById(task->getRelatedCourseId()).isEmpty()
                                                        ? "未关联课程"
                                                        : manager->getCourseNameById(task->getRelatedCourseId())));
        table->setItem(row, 3, new QTableWidgetItem(task->getDeadline().toString("yyyy-MM-dd")));
        table->setItem(row, 4, new QTableWidgetItem(priorityText(task->getPriority())));
    }
    table->resizeColumnsToContents();
}

void PlanPage::refresh()
{
    fillTaskTable(todayTable, manager->getTodayPlan());
    fillTaskTable(weekTable, manager->getWeekPlan());

    reminderList->clear();
    const QList<BaseTask*> deadlineTasks = manager->getDeadlineReminders();
    const QList<BaseTask*> highPriorityTasks = manager->getHighPriorityReminders();

    for (BaseTask* task : deadlineTasks) {
        reminderList->addItem("临近截止：" + task->getDisplayText());
    }
    for (BaseTask* task : highPriorityTasks) {
        reminderList->addItem("高优先级：" + task->getDisplayText());
    }
    if (reminderList->count() == 0) {
        reminderList->addItem("暂无需要提醒的任务。");
    }
}
