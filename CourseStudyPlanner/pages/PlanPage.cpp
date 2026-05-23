#include "PlanPage.h"

#include "UiTheme.h"

#include <QAbstractItemView>
#include <QFrame>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

namespace {
QString priorityText(int priority)
{
    if (priority == 3) {
        return "高";
    }
    if (priority == 2) {
        return "中";
    }
    return "低";
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
    QVBoxLayout* pageLayout = new QVBoxLayout(this);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->setSpacing(18);
    pageLayout->setSizeConstraint(QLayout::SetMinimumSize);

    QFrame* introCard = UiTheme::createCard();
    QVBoxLayout* introLayout = new QVBoxLayout(introCard);
    introLayout->setContentsMargins(24, 22, 24, 22);
    introLayout->setSpacing(16);

    QLabel* eyebrow = new QLabel("计划视图");
    eyebrow->setProperty("role", "eyebrow");

    QLabel* titleLabel = new QLabel("把零散任务整理成今天、本周和提醒三层视图");
    titleLabel->setProperty("role", "pageTitle");
    titleLabel->setWordWrap(true);

    QLabel* subtitleLabel = new QLabel("内容区只展示真正需要执行的计划信息，提醒区单独放在侧边，避免所有任务堆在一个列表里。");
    subtitleLabel->setProperty("role", "muted");
    subtitleLabel->setWordWrap(true);

    todayCountLabel = new QLabel;
    todayCountLabel->setProperty("role", "metricValue");
    todayCountLabel->setStyleSheet("font-size: 24px;");

    weekCountLabel = new QLabel;
    weekCountLabel->setProperty("role", "metricValue");
    weekCountLabel->setStyleSheet("font-size: 24px;");

    reminderCountLabel = new QLabel;
    reminderCountLabel->setProperty("role", "metricValue");
    reminderCountLabel->setStyleSheet("font-size: 24px;");

    QLabel* todayCaption = new QLabel("今日任务");
    todayCaption->setProperty("role", "metricCaption");
    QLabel* weekCaption = new QLabel("本周任务");
    weekCaption->setProperty("role", "metricCaption");
    QLabel* reminderCaption = new QLabel("提醒数量");
    reminderCaption->setProperty("role", "metricCaption");

    QFrame* todayMetricCard = UiTheme::createCard();
    QVBoxLayout* todayMetricLayout = new QVBoxLayout(todayMetricCard);
    todayMetricLayout->setContentsMargins(18, 16, 18, 16);
    todayMetricLayout->addWidget(todayCountLabel);
    todayMetricLayout->addWidget(todayCaption);

    QFrame* weekMetricCard = UiTheme::createCard();
    QVBoxLayout* weekMetricLayout = new QVBoxLayout(weekMetricCard);
    weekMetricLayout->setContentsMargins(18, 16, 18, 16);
    weekMetricLayout->addWidget(weekCountLabel);
    weekMetricLayout->addWidget(weekCaption);

    QFrame* reminderMetricCard = UiTheme::createCard();
    QVBoxLayout* reminderMetricLayout = new QVBoxLayout(reminderMetricCard);
    reminderMetricLayout->setContentsMargins(18, 16, 18, 16);
    reminderMetricLayout->addWidget(reminderCountLabel);
    reminderMetricLayout->addWidget(reminderCaption);

    QHBoxLayout* metricLayout = new QHBoxLayout;
    metricLayout->setSpacing(12);
    metricLayout->addWidget(todayMetricCard);
    metricLayout->addWidget(weekMetricCard);
    metricLayout->addWidget(reminderMetricCard);
    metricLayout->addStretch();

    introLayout->addWidget(eyebrow);
    introLayout->addWidget(titleLabel);
    introLayout->addWidget(subtitleLabel);
    introLayout->addLayout(metricLayout);

    todayTable = new QTableWidget;
    weekTable = new QTableWidget;
    reminderList = new QListWidget;

    for (QTableWidget* table : { todayTable, weekTable }) {
        table->setColumnCount(5);
        table->setHorizontalHeaderLabels({ "标题", "类型", "所属课程", "截止日期", "优先级" });
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->horizontalHeader()->setStretchLastSection(true);
        table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        UiTheme::polishTable(table);
    }

    QFrame* todayCard = UiTheme::createCard();
    QVBoxLayout* todayLayout = new QVBoxLayout(todayCard);
    todayLayout->setContentsMargins(20, 20, 20, 20);
    todayLayout->setSpacing(12);
    QLabel* todayTitle = new QLabel("主任务区");
    todayTitle->setProperty("role", "sectionTitle");
    QLabel* todayHint = new QLabel("只展示今天到期的任务，帮助你快速确认最先该做什么。");
    todayHint->setProperty("role", "muted");
    todayHint->setWordWrap(true);
    todayLayout->addWidget(todayTitle);
    todayLayout->addWidget(todayHint);
    todayLayout->addWidget(todayTable, 1);

    QFrame* reminderCard = UiTheme::createCard();
    QVBoxLayout* reminderLayout = new QVBoxLayout(reminderCard);
    reminderLayout->setContentsMargins(20, 20, 20, 20);
    reminderLayout->setSpacing(12);
    QLabel* reminderTitle = new QLabel("提醒区");
    reminderTitle->setProperty("role", "sectionTitle");
    QLabel* reminderHint = new QLabel("把临近截止和高优先级任务集中展示，避免遗漏关键事项。");
    reminderHint->setProperty("role", "muted");
    reminderHint->setWordWrap(true);
    reminderLayout->addWidget(reminderTitle);
    reminderLayout->addWidget(reminderHint);
    reminderLayout->addWidget(reminderList, 1);

    QHBoxLayout* upperBoardLayout = new QHBoxLayout;
    upperBoardLayout->setSpacing(18);
    upperBoardLayout->addWidget(todayCard, 3);
    upperBoardLayout->addWidget(reminderCard, 2);

    QFrame* weekCard = UiTheme::createCard();
    QVBoxLayout* weekLayout = new QVBoxLayout(weekCard);
    weekLayout->setContentsMargins(20, 20, 20, 20);
    weekLayout->setSpacing(12);
    QLabel* weekTitle = new QLabel("本周视图");
    weekTitle->setProperty("role", "sectionTitle");
    QLabel* weekHint = new QLabel("除了今日重点，这里还会展示未来一周内需要关注的任务。");
    weekHint->setProperty("role", "muted");
    weekHint->setWordWrap(true);
    weekLayout->addWidget(weekTitle);
    weekLayout->addWidget(weekHint);
    weekLayout->addWidget(weekTable, 1);

    pageLayout->addWidget(introCard);
    pageLayout->addLayout(upperBoardLayout, 1);
    pageLayout->addWidget(weekCard, 1);
}

void PlanPage::fillTaskTable(QTableWidget* table, const QList<BaseTask*>& tasks)
{
    table->setRowCount(tasks.size());
    for (int row = 0; row < tasks.size(); ++row) {
        BaseTask* task = tasks[row];
        const QString courseName = manager->getCourseNameById(task->getRelatedCourseId());

        table->setItem(row, 0, new QTableWidgetItem(task->getTitle()));
        table->setItem(row, 1, new QTableWidgetItem(typeText(task->getType())));
        table->setItem(row, 2, new QTableWidgetItem(courseName.isEmpty() ? "未关联" : courseName));
        table->setItem(row, 3, new QTableWidgetItem(task->getDeadline().toString("yyyy-MM-dd")));
        table->setItem(row, 4, new QTableWidgetItem(priorityText(task->getPriority())));
    }
    table->resizeColumnsToContents();
}

void PlanPage::refresh()
{
    const QList<BaseTask*> todayPlan = manager->getTodayPlan();
    const QList<BaseTask*> weekPlan = manager->getWeekPlan();
    fillTaskTable(todayTable, todayPlan);
    fillTaskTable(weekTable, weekPlan);

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
        reminderList->addItem("当前没有需要特别关注的提醒。");
    }

    todayCountLabel->setText(QString::number(todayPlan.size()));
    weekCountLabel->setText(QString::number(weekPlan.size()));
    reminderCountLabel->setText(QString::number(deadlineTasks.size() + highPriorityTasks.size()));
}
