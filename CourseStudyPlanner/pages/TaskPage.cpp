#include "TaskPage.h"

#include "UiTheme.h"

#include "../models/Plan.h"

#include <QAbstractItemView>
#include <QCheckBox>
#include <QDateEdit>
#include <QDialog>
#include <QFormLayout>
#include <QFrame>
#include <QGridLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSizePolicy>
#include <QTextEdit>
#include <QVBoxLayout>

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
        return "高";
    }
    if (priority == 2) {
        return "中";
    }
    return "低";
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
    QVBoxLayout* pageLayout = new QVBoxLayout(this);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->setSpacing(18);
    pageLayout->setSizeConstraint(QLayout::SetMinimumSize);

    QFrame* introCard = UiTheme::createCard();
    QVBoxLayout* introLayout = new QVBoxLayout(introCard);
    introLayout->setContentsMargins(24, 22, 24, 22);
    introLayout->setSpacing(16);

    QLabel* eyebrow = new QLabel("任务执行台");
    eyebrow->setProperty("role", "eyebrow");

    QLabel* titleLabel = new QLabel("把任务处理、筛选与状态切换集中到一个操作界面");
    titleLabel->setProperty("role", "pageTitle");
    titleLabel->setWordWrap(true);

    QLabel* subtitleLabel = new QLabel("页头只放主操作和关键指标，中间保留任务主列表，右侧辅助区集中展示筛选器和说明，信息层级更清楚。");
    subtitleLabel->setProperty("role", "muted");
    subtitleLabel->setWordWrap(true);

    totalTasksLabel = new QLabel;
    totalTasksLabel->setProperty("role", "metricValue");
    totalTasksLabel->setStyleSheet("font-size: 22px;");

    openTasksLabel = new QLabel;
    openTasksLabel->setProperty("role", "metricValue");
    openTasksLabel->setStyleSheet("font-size: 22px;");

    highPriorityLabel = new QLabel;
    highPriorityLabel->setProperty("role", "metricValue");
    highPriorityLabel->setStyleSheet("font-size: 22px;");

    QLabel* totalCaption = new QLabel("任务总数");
    totalCaption->setProperty("role", "metricCaption");
    QLabel* openCaption = new QLabel("未完成");
    openCaption->setProperty("role", "metricCaption");
    QLabel* highCaption = new QLabel("高优先级");
    highCaption->setProperty("role", "metricCaption");

    QFrame* totalCard = UiTheme::createCard();
    QVBoxLayout* totalLayout = new QVBoxLayout(totalCard);
    totalLayout->setContentsMargins(18, 16, 18, 16);
    totalCard->setMinimumWidth(128);
    totalLayout->addWidget(totalTasksLabel);
    totalLayout->addWidget(totalCaption);

    QFrame* openCard = UiTheme::createCard();
    QVBoxLayout* openLayout = new QVBoxLayout(openCard);
    openLayout->setContentsMargins(18, 16, 18, 16);
    openCard->setMinimumWidth(128);
    openLayout->addWidget(openTasksLabel);
    openLayout->addWidget(openCaption);

    QFrame* highCard = UiTheme::createCard();
    QVBoxLayout* highLayout = new QVBoxLayout(highCard);
    highLayout->setContentsMargins(18, 16, 18, 16);
    highCard->setMinimumWidth(128);
    highLayout->addWidget(highPriorityLabel);
    highLayout->addWidget(highCaption);

    QPushButton* addButton = new QPushButton("新增任务");
    UiTheme::setButtonVariant(addButton, "primary");
    addButton->setMinimumWidth(128);

    QPushButton* editButton = new QPushButton("编辑所选任务");
    UiTheme::setButtonVariant(editButton, "secondary");
    editButton->setMinimumWidth(128);
    QPushButton* toggleButton = new QPushButton("切换完成状态");
    toggleButton->setMinimumWidth(128);
    QPushButton* deleteButton = new QPushButton("删除所选任务");
    UiTheme::setButtonVariant(deleteButton, "danger");
    deleteButton->setMinimumWidth(128);

    for (QPushButton* button : { addButton, editButton, toggleButton, deleteButton }) {
        button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    }

    QHBoxLayout* metricsLayout = new QHBoxLayout;
    metricsLayout->setSpacing(12);
    metricsLayout->addWidget(totalCard);
    metricsLayout->addWidget(openCard);
    metricsLayout->addWidget(highCard);
    metricsLayout->addStretch();

    QHBoxLayout* actionLayout = new QHBoxLayout;
    actionLayout->setSpacing(12);
    actionLayout->addWidget(editButton);
    actionLayout->addWidget(toggleButton);
    actionLayout->addWidget(deleteButton);
    actionLayout->addWidget(addButton);
    actionLayout->addStretch();

    introLayout->addWidget(eyebrow);
    introLayout->addWidget(titleLabel);
    introLayout->addWidget(subtitleLabel);
    introLayout->addLayout(metricsLayout);
    introLayout->addLayout(actionLayout);

    QFrame* filterCard = UiTheme::createCard();
    QGridLayout* filterLayout = new QGridLayout(filterCard);
    filterLayout->setContentsMargins(20, 18, 20, 18);
    filterLayout->setHorizontalSpacing(16);
    filterLayout->setVerticalSpacing(12);

    QLabel* filterTitle = new QLabel("辅助筛选");
    filterTitle->setProperty("role", "sectionTitle");

    QLabel* filterHint = new QLabel("按课程、任务类型、完成状态和排序方式组合筛选，减少在长列表里查找的成本。");
    filterHint->setProperty("role", "muted");
    filterHint->setWordWrap(true);

    courseFilterCombo = new QComboBox;
    typeFilterCombo = new QComboBox;
    stateFilterCombo = new QComboBox;
    sortCombo = new QComboBox;

    typeFilterCombo->addItem("全部类型", "");
    typeFilterCombo->addItem("课程作业", "Homework");
    typeFilterCombo->addItem("自主学习", "SelfStudy");
    typeFilterCombo->addItem("考试复习", "Exam");

    stateFilterCombo->addItem("全部状态", "all");
    stateFilterCombo->addItem("仅看未完成", "unfinished");
    stateFilterCombo->addItem("仅看已完成", "completed");

    sortCombo->addItem("按截止日期排序", "deadline");
    sortCombo->addItem("按优先级排序", "priority");

    filterLayout->addWidget(filterTitle, 0, 0, 1, 2);
    filterLayout->addWidget(filterHint, 1, 0, 1, 2);
    filterLayout->addWidget(new QLabel("课程"), 2, 0);
    filterLayout->addWidget(courseFilterCombo, 3, 0);
    filterLayout->addWidget(new QLabel("任务类型"), 2, 1);
    filterLayout->addWidget(typeFilterCombo, 3, 1);
    filterLayout->addWidget(new QLabel("完成状态"), 4, 0);
    filterLayout->addWidget(stateFilterCombo, 5, 0);
    filterLayout->addWidget(new QLabel("排序方式"), 4, 1);
    filterLayout->addWidget(sortCombo, 5, 1);

    QFrame* helperCard = UiTheme::createCard();
    QVBoxLayout* helperLayout = new QVBoxLayout(helperCard);
    helperLayout->setContentsMargins(20, 20, 20, 20);
    helperLayout->setSpacing(12);

    QLabel* helperTitle = new QLabel("使用建议");
    helperTitle->setProperty("role", "sectionTitle");

    QLabel* helperText = new QLabel("任务页是最主要的执行面板。建议优先通过筛选缩小范围，再执行编辑、删除或完成切换。");
    helperText->setProperty("role", "muted");
    helperText->setWordWrap(true);

    QLabel* helperPointOne = new QLabel("1. 高优先级任务建议优先标记和处理。");
    helperPointOne->setProperty("role", "muted");
    helperPointOne->setWordWrap(true);

    QLabel* helperPointTwo = new QLabel("2. 未关联课程的任务会显示为“未关联”。");
    helperPointTwo->setProperty("role", "muted");
    helperPointTwo->setWordWrap(true);

    QLabel* helperPointThree = new QLabel("3. 编辑任务时可直接修改完成状态。");
    helperPointThree->setProperty("role", "muted");
    helperPointThree->setWordWrap(true);

    helperLayout->addWidget(helperTitle);
    helperLayout->addWidget(helperText);
    helperLayout->addWidget(helperPointOne);
    helperLayout->addWidget(helperPointTwo);
    helperLayout->addWidget(helperPointThree);
    helperLayout->addStretch();

    QFrame* tableCard = UiTheme::createCard();
    QVBoxLayout* tableLayout = new QVBoxLayout(tableCard);
    tableLayout->setContentsMargins(20, 20, 20, 20);
    tableLayout->setSpacing(12);

    QLabel* tableTitle = new QLabel("任务总览");
    tableTitle->setProperty("role", "sectionTitle");

    QLabel* tableHint = new QLabel("表格只负责展示任务详情，所有主操作都集中在页头，辅助筛选放在右侧。");
    tableHint->setProperty("role", "muted");
    tableHint->setWordWrap(true);

    table = new QTableWidget;
    table->setColumnCount(8);
    table->setHorizontalHeaderLabels({ "任务ID", "标题", "类型", "所属课程", "截止日期", "优先级", "状态", "描述" });
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->setColumnHidden(0, true);
    UiTheme::polishTable(table);

    tableLayout->addWidget(tableTitle);
    tableLayout->addWidget(tableHint);
    tableLayout->addWidget(table, 1);

    QVBoxLayout* sideColumnLayout = new QVBoxLayout;
    sideColumnLayout->setSpacing(18);
    sideColumnLayout->addWidget(filterCard);
    sideColumnLayout->addWidget(helperCard);
    sideColumnLayout->addStretch();

    QHBoxLayout* bodyLayout = new QHBoxLayout;
    bodyLayout->setSpacing(18);
    bodyLayout->addWidget(tableCard, 1);
    bodyLayout->addLayout(sideColumnLayout);

    pageLayout->addWidget(introCard);
    pageLayout->addLayout(bodyLayout, 1);

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
    int openCount = 0;
    int highPriorityCount = 0;

    for (BaseTask* task : tasks) {
        if (!task) {
            continue;
        }

        if (!task->getIsCompleted()) {
            ++openCount;
        }
        if (task->getPriority() == 3 && !task->getIsCompleted()) {
            ++highPriorityCount;
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
        const QString courseName = manager->getCourseNameById(task->getRelatedCourseId());

        table->setItem(row, 0, new QTableWidgetItem(task->getTaskId()));
        table->setItem(row, 1, new QTableWidgetItem(task->getTitle()));
        table->setItem(row, 2, new QTableWidgetItem(typeToText(task->getType())));
        table->setItem(row, 3, new QTableWidgetItem(courseName.isEmpty() ? "未关联" : courseName));
        table->setItem(row, 4, new QTableWidgetItem(task->getDeadline().toString("yyyy-MM-dd")));
        table->setItem(row, 5, new QTableWidgetItem(priorityToText(task->getPriority())));
        table->setItem(row, 6, new QTableWidgetItem(task->getIsCompleted() ? "已完成" : "未完成"));
        table->setItem(row, 7, new QTableWidgetItem(task->getDescription()));
    }

    totalTasksLabel->setText(QString::number(tasks.size()));
    openTasksLabel->setText(QString::number(openCount));
    highPriorityLabel->setText(QString::number(highPriorityCount));
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
    dialog.resize(520, 500);

    QVBoxLayout* rootLayout = new QVBoxLayout(&dialog);
    rootLayout->setContentsMargins(24, 24, 24, 24);
    rootLayout->setSpacing(16);

    QLabel* dialogTitle = new QLabel(title);
    dialogTitle->setProperty("role", "sectionTitle");

    QLabel* dialogSubtitle = new QLabel("任务信息建议尽量完整，但保持简洁，这样主列表会更容易快速浏览。");
    dialogSubtitle->setProperty("role", "muted");
    dialogSubtitle->setWordWrap(true);

    QLineEdit* titleEdit = new QLineEdit(task ? task->getTitle() : "");
    titleEdit->setPlaceholderText("请输入任务标题");

    QTextEdit* descriptionEdit = new QTextEdit(task ? task->getDescription() : "");
    descriptionEdit->setPlaceholderText("请输入任务说明、作业要求或学习范围");
    descriptionEdit->setFixedHeight(120);

    QDateEdit* deadlineEdit = new QDateEdit(task ? task->getDeadline() : QDate::currentDate());
    deadlineEdit->setCalendarPopup(true);
    deadlineEdit->setDisplayFormat("yyyy-MM-dd");

    QComboBox* priorityCombo = new QComboBox;
    priorityCombo->addItem("低", 1);
    priorityCombo->addItem("中", 2);
    priorityCombo->addItem("高", 3);
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
    courseCombo->addItem("未关联", "");
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

    QCheckBox* completedCheckBox = new QCheckBox("标记为已完成");
    completedCheckBox->setChecked(task ? task->getIsCompleted() : false);
    completedCheckBox->setEnabled(task != nullptr);

    QFormLayout* formLayout = new QFormLayout;
    formLayout->setLabelAlignment(Qt::AlignLeft);
    formLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    formLayout->setHorizontalSpacing(16);
    formLayout->setVerticalSpacing(14);
    formLayout->addRow("标题", titleEdit);
    formLayout->addRow("描述", descriptionEdit);
    formLayout->addRow("截止日期", deadlineEdit);
    formLayout->addRow("优先级", priorityCombo);
    formLayout->addRow("任务类型", typeCombo);
    formLayout->addRow("所属课程", courseCombo);
    formLayout->addRow("状态", completedCheckBox);

    QPushButton* cancelButton = new QPushButton("取消");
    QPushButton* okButton = new QPushButton("保存任务");
    UiTheme::setButtonVariant(okButton, "primary");

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(okButton);

    rootLayout->addWidget(dialogTitle);
    rootLayout->addWidget(dialogSubtitle);
    rootLayout->addLayout(formLayout);
    rootLayout->addStretch();
    rootLayout->addLayout(buttonLayout);

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

    if (!showTaskDialog("新增任务", nullptr, title, description, deadline, priority, relatedCourseId, taskType, isCompleted)) {
        return;
    }

    QString errorMessage;
    if (!manager->addTask(title, description, deadline, priority, relatedCourseId, taskType, errorMessage)) {
        QMessageBox::warning(this, "新增失败", errorMessage);
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
        QMessageBox::information(this, "需要先选择任务", "请先选择一条任务再进行编辑。");
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
        QMessageBox::information(this, "需要先选择任务", "请先选择一条任务再进行删除。");
        return;
    }
    if (QMessageBox::question(this, "删除任务", "确认删除所选任务吗？") != QMessageBox::Yes) {
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
        QMessageBox::information(this, "需要先选择任务", "请先选择一条任务再切换完成状态。");
        return;
    }

    QString errorMessage;
    if (!manager->toggleTaskCompleted(taskId, errorMessage)) {
        QMessageBox::warning(this, "更新失败", errorMessage);
        return;
    }
    refresh();
    emit dataChanged();
}
