#include "TaskPage.h"

#include "../models/Plan.h"

#include <QAbstractItemView>
#include <QCheckBox>
#include <QDateEdit>
#include <QDialog>
#include <QFormLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

#include <algorithm>

namespace {
QString typeToText(const QString& type)
{
    if (type == "Homework") {
        return "课程作业";
    }
    if (type == "SelfStudy") {
        return "自主学习";
    }
    return "考试复习";
}

QString priorityToText(int priority)
{
    if (priority == 3) {
        return "High";
    }
    if (priority == 2) {
        return "Medium";
    }
    return "Low";
}

int priorityToIndex(int priority)
{
    if (priority == 3) {
        return 2;
    }
    if (priority == 2) {
        return 1;
    }
    return 0;
}
}

TaskPage::TaskPage(SystemManager* manager, QWidget* parent)
    : QWidget(parent), manager(manager)
{
    QLabel* titleLabel = new QLabel("任务管理");
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");

    courseFilterCombo = new QComboBox;
    typeFilterCombo = new QComboBox;
    stateFilterCombo = new QComboBox;
    sortCombo = new QComboBox;

    typeFilterCombo->addItem("全部类型", "");
    typeFilterCombo->addItem("课程作业", "Homework");
    typeFilterCombo->addItem("自主学习", "SelfStudy");
    typeFilterCombo->addItem("考试复习", "Exam");

    stateFilterCombo->addItem("全部状态", "all");
    stateFilterCombo->addItem("未完成", "unfinished");
    stateFilterCombo->addItem("已完成", "completed");

    sortCombo->addItem("按截止日期排序", "deadline");
    sortCombo->addItem("按优先级排序", "priority");

    QHBoxLayout* filterLayout = new QHBoxLayout;
    filterLayout->addWidget(new QLabel("课程："));
    filterLayout->addWidget(courseFilterCombo);
    filterLayout->addWidget(new QLabel("类型："));
    filterLayout->addWidget(typeFilterCombo);
    filterLayout->addWidget(new QLabel("状态："));
    filterLayout->addWidget(stateFilterCombo);
    filterLayout->addWidget(new QLabel("排序："));
    filterLayout->addWidget(sortCombo);
    filterLayout->addStretch();

    QPushButton* addButton = new QPushButton("添加任务");
    QPushButton* editButton = new QPushButton("编辑任务");
    QPushButton* deleteButton = new QPushButton("删除任务");
    QPushButton* toggleButton = new QPushButton("完成 / 未完成");

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(toggleButton);
    buttonLayout->addStretch();

    table = new QTableWidget;
    table->setColumnCount(8);
    table->setHorizontalHeaderLabels({ "任务ID", "标题", "类型", "课程", "截止日期", "优先级", "状态", "描述" });
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setColumnHidden(0, true);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addLayout(filterLayout);
    layout->addLayout(buttonLayout);
    layout->addWidget(table);

    connect(addButton, &QPushButton::clicked, this, &TaskPage::addTask);
    connect(editButton, &QPushButton::clicked, this, &TaskPage::editTask);
    connect(deleteButton, &QPushButton::clicked, this, &TaskPage::deleteTask);
    connect(toggleButton, &QPushButton::clicked, this, &TaskPage::toggleTaskCompleted);
    connect(courseFilterCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &TaskPage::refresh);
    connect(typeFilterCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &TaskPage::refresh);
    connect(stateFilterCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &TaskPage::refresh);
    connect(sortCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &TaskPage::refresh);
}

void TaskPage::refreshCourseFilter()
{
    const QString oldValue = courseFilterCombo->currentData().toString();
    courseFilterCombo->blockSignals(true);
    courseFilterCombo->clear();
    courseFilterCombo->addItem("全部课程", "");
    courseFilterCombo->addItem("未关联课程", "__none__");

    const QList<Course> courses = manager->getCurrentUserCourses();
    for (const Course& course : courses) {
        courseFilterCombo->addItem(course.getCourseName(), course.getCourseId());
    }

    const int index = courseFilterCombo->findData(oldValue);
    if (index >= 0) {
        courseFilterCombo->setCurrentIndex(index);
    }
    courseFilterCombo->blockSignals(false);
}

void TaskPage::refresh()
{
    refreshCourseFilter();

    QList<BaseTask*> tasks = manager->getCurrentUserTasks();
    const QString courseFilter = courseFilterCombo->currentData().toString();
    const QString typeFilter = typeFilterCombo->currentData().toString();
    const QString stateFilter = stateFilterCombo->currentData().toString();

    QList<BaseTask*> filtered;
    for (BaseTask* task : tasks) {
        if (!task) {
            continue;
        }
        if (!typeFilter.isEmpty() && task->getType() != typeFilter) {
            continue;
        }
        if (courseFilter == "__none__" && !task->getRelatedCourseId().isEmpty()) {
            continue;
        }
        if (!courseFilter.isEmpty() && courseFilter != "__none__" && task->getRelatedCourseId() != courseFilter) {
            continue;
        }
        if (stateFilter == "completed" && !task->getIsCompleted()) {
            continue;
        }
        if (stateFilter == "unfinished" && task->getIsCompleted()) {
            continue;
        }
        filtered.append(task);
    }

    if (sortCombo->currentData().toString() == "priority") {
        Plan::sortByPriority(filtered);
    } else {
        Plan::sortByDeadline(filtered);
    }

    table->setRowCount(filtered.size());
    for (int row = 0; row < filtered.size(); ++row) {
        BaseTask* task = filtered[row];
        table->setItem(row, 0, new QTableWidgetItem(task->getTaskId()));
        table->setItem(row, 1, new QTableWidgetItem(task->getTitle()));
        table->setItem(row, 2, new QTableWidgetItem(typeToText(task->getType())));
        table->setItem(row, 3, new QTableWidgetItem(manager->getCourseNameById(task->getRelatedCourseId()).isEmpty()
                                                        ? "未关联课程"
                                                        : manager->getCourseNameById(task->getRelatedCourseId())));
        table->setItem(row, 4, new QTableWidgetItem(task->getDeadline().toString("yyyy-MM-dd")));
        table->setItem(row, 5, new QTableWidgetItem(priorityToText(task->getPriority())));
        table->setItem(row, 6, new QTableWidgetItem(task->getIsCompleted() ? "已完成" : "未完成"));
        table->setItem(row, 7, new QTableWidgetItem(task->getDescription()));
    }
    table->resizeColumnsToContents();
}

QString TaskPage::selectedTaskId() const
{
    const int row = table->currentRow();
    if (row < 0 || !table->item(row, 0)) {
        return "";
    }
    return table->item(row, 0)->text();
}

BaseTask* TaskPage::findTaskById(const QString& taskId) const
{
    const QList<BaseTask*> tasks = manager->getCurrentUserTasks();
    for (BaseTask* task : tasks) {
        if (task && task->getTaskId() == taskId) {
            return task;
        }
    }
    return nullptr;
}

bool TaskPage::showTaskDialog(const QString& title,
                              BaseTask* task,
                              QString& taskTitle,
                              QString& description,
                              QDate& deadline,
                              int& priority,
                              QString& relatedCourseId,
                              QString& taskType,
                              bool& isCompleted)
{
    QDialog dialog(this);
    dialog.setWindowTitle(title);
    dialog.resize(420, 360);

    QLineEdit* titleEdit = new QLineEdit(task ? task->getTitle() : "");
    QTextEdit* descriptionEdit = new QTextEdit(task ? task->getDescription() : "");
    descriptionEdit->setFixedHeight(80);

    QDateEdit* deadlineEdit = new QDateEdit(task ? task->getDeadline() : QDate::currentDate());
    deadlineEdit->setCalendarPopup(true);
    deadlineEdit->setDisplayFormat("yyyy-MM-dd");

    QComboBox* priorityCombo = new QComboBox;
    priorityCombo->addItem("Low", 1);
    priorityCombo->addItem("Medium", 2);
    priorityCombo->addItem("High", 3);
    priorityCombo->setCurrentIndex(task ? priorityToIndex(task->getPriority()) : 1);

    QComboBox* typeCombo = new QComboBox;
    typeCombo->addItem("课程作业", "Homework");
    typeCombo->addItem("自主学习", "SelfStudy");
    typeCombo->addItem("考试复习", "Exam");
    if (task) {
        const int typeIndex = typeCombo->findData(task->getType());
        if (typeIndex >= 0) {
            typeCombo->setCurrentIndex(typeIndex);
        }
    }

    QComboBox* courseCombo = new QComboBox;
    courseCombo->addItem("未关联课程", "");
    const QList<Course> courses = manager->getCurrentUserCourses();
    for (const Course& course : courses) {
        courseCombo->addItem(course.getCourseName(), course.getCourseId());
    }
    if (task) {
        const int courseIndex = courseCombo->findData(task->getRelatedCourseId());
        if (courseIndex >= 0) {
            courseCombo->setCurrentIndex(courseIndex);
        }
    }

    QCheckBox* completedCheckBox = new QCheckBox("已完成");
    completedCheckBox->setChecked(task ? task->getIsCompleted() : false);
    completedCheckBox->setEnabled(task != nullptr);

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("任务标题：", titleEdit);
    formLayout->addRow("任务描述：", descriptionEdit);
    formLayout->addRow("截止日期：", deadlineEdit);
    formLayout->addRow("优先级：", priorityCombo);
    formLayout->addRow("任务类型：", typeCombo);
    formLayout->addRow("所属课程：", courseCombo);
    formLayout->addRow("完成状态：", completedCheckBox);

    QPushButton* okButton = new QPushButton("保存");
    QPushButton* cancelButton = new QPushButton("取消");
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    layout->addLayout(formLayout);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return false;
    }

    taskTitle = titleEdit->text();
    description = descriptionEdit->toPlainText();
    deadline = deadlineEdit->date();
    priority = priorityCombo->currentData().toInt();
    relatedCourseId = courseCombo->currentData().toString();
    taskType = typeCombo->currentData().toString();
    isCompleted = completedCheckBox->isChecked();
    return true;
}

void TaskPage::addTask()
{
    QString title;
    QString description;
    QDate deadline;
    int priority = 2;
    QString relatedCourseId;
    QString taskType;
    bool isCompleted = false;

    if (!showTaskDialog("添加任务", nullptr, title, description, deadline, priority, relatedCourseId, taskType, isCompleted)) {
        return;
    }

    QString errorMessage;
    if (!manager->addTask(title, description, deadline, priority, relatedCourseId, taskType, errorMessage)) {
        QMessageBox::warning(this, "添加失败", errorMessage);
        return;
    }
    refresh();
    emit dataChanged();
}

void TaskPage::editTask()
{
    const QString taskId = selectedTaskId();
    BaseTask* task = findTaskById(taskId);
    if (!task) {
        QMessageBox::information(this, "提示", "请先选择一个任务。");
        return;
    }

    QString title;
    QString description;
    QDate deadline;
    int priority = 2;
    QString relatedCourseId;
    QString taskType;
    bool isCompleted = false;

    if (!showTaskDialog("编辑任务", task, title, description, deadline, priority, relatedCourseId, taskType, isCompleted)) {
        return;
    }

    QString errorMessage;
    if (!manager->updateTask(taskId, title, description, deadline, priority, isCompleted,
                             relatedCourseId, taskType, errorMessage)) {
        QMessageBox::warning(this, "编辑失败", errorMessage);
        return;
    }
    refresh();
    emit dataChanged();
}

void TaskPage::deleteTask()
{
    const QString taskId = selectedTaskId();
    if (taskId.isEmpty()) {
        QMessageBox::information(this, "提示", "请先选择一个任务。");
        return;
    }
    if (QMessageBox::question(this, "确认删除", "确定要删除选中的任务吗？") != QMessageBox::Yes) {
        return;
    }

    QString errorMessage;
    if (!manager->deleteTask(taskId, errorMessage)) {
        QMessageBox::warning(this, "删除失败", errorMessage);
        return;
    }
    refresh();
    emit dataChanged();
}

void TaskPage::toggleTaskCompleted()
{
    const QString taskId = selectedTaskId();
    if (taskId.isEmpty()) {
        QMessageBox::information(this, "提示", "请先选择一个任务。");
        return;
    }

    QString errorMessage;
    if (!manager->toggleTaskCompleted(taskId, errorMessage)) {
        QMessageBox::warning(this, "操作失败", errorMessage);
        return;
    }
    refresh();
    emit dataChanged();
}
