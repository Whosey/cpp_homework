#include "StatsPage.h"

#include "UiTheme.h"

#include <QAbstractItemView>
#include <QFrame>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidgetItem>
#include <QVBoxLayout>

StatsPage::StatsPage(SystemManager* manager, QWidget* parent)
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

    QLabel* eyebrow = new QLabel("统计分析");
    eyebrow->setProperty("role", "eyebrow");

    QLabel* titleLabel = new QLabel("从完成率、任务规模和课程分布回看执行情况");
    titleLabel->setProperty("role", "pageTitle");
    titleLabel->setWordWrap(true);

    QLabel* subtitleLabel = new QLabel("统计页保留结果视角：主内容区展示关键指标和课程分布，辅助区提供阅读提示，避免和任务页功能重叠。");
    subtitleLabel->setProperty("role", "muted");
    subtitleLabel->setWordWrap(true);

    totalLabel = new QLabel("0");
    totalLabel->setProperty("role", "metricValue");
    completedLabel = new QLabel("0");
    completedLabel->setProperty("role", "metricValue");
    unfinishedLabel = new QLabel("0");
    unfinishedLabel->setProperty("role", "metricValue");
    weekLabel = new QLabel("0%");
    weekLabel->setProperty("role", "metricValue");

    auto createMetricCard = [](QLabel* valueLabel, const QString& caption) {
        QFrame* card = UiTheme::createCard();
        QVBoxLayout* layout = new QVBoxLayout(card);
        layout->setContentsMargins(18, 16, 18, 16);
        layout->setSpacing(4);

        QLabel* captionLabel = new QLabel(caption);
        captionLabel->setProperty("role", "metricCaption");

        layout->addWidget(valueLabel);
        layout->addWidget(captionLabel);
        return card;
    };

    QHBoxLayout* metricLayout = new QHBoxLayout;
    metricLayout->setSpacing(12);
    metricLayout->addWidget(createMetricCard(totalLabel, "任务总数"));
    metricLayout->addWidget(createMetricCard(completedLabel, "已完成"));
    metricLayout->addWidget(createMetricCard(unfinishedLabel, "未完成"));
    metricLayout->addWidget(createMetricCard(weekLabel, "本周完成率"));

    introLayout->addWidget(eyebrow);
    introLayout->addWidget(titleLabel);
    introLayout->addWidget(subtitleLabel);
    introLayout->addLayout(metricLayout);

    QFrame* progressCard = UiTheme::createCard();
    QVBoxLayout* progressLayout = new QVBoxLayout(progressCard);
    progressLayout->setContentsMargins(20, 20, 20, 20);
    progressLayout->setSpacing(10);

    QLabel* progressTitle = new QLabel("进度概览");
    progressTitle->setProperty("role", "sectionTitle");

    QLabel* progressHint = new QLabel("这里显示本周完成进度，帮助你快速判断当前执行节奏是否达标。");
    progressHint->setProperty("role", "muted");
    progressHint->setWordWrap(true);

    completionProgress = new QProgressBar;
    completionProgress->setRange(0, 100);
    completionProgress->setFormat("本周完成率：%p%");

    progressLayout->addWidget(progressTitle);
    progressLayout->addWidget(progressHint);
    progressLayout->addWidget(completionProgress);

    QFrame* courseCard = UiTheme::createCard();
    QVBoxLayout* courseLayout = new QVBoxLayout(courseCard);
    courseLayout->setContentsMargins(20, 20, 20, 20);
    courseLayout->setSpacing(12);

    QLabel* courseTitle = new QLabel("课程任务分布");
    courseTitle->setProperty("role", "sectionTitle");

    QLabel* courseHint = new QLabel("按课程统计任务数量，帮助你判断任务负载是否集中在某几门课。");
    courseHint->setProperty("role", "muted");
    courseHint->setWordWrap(true);

    courseStatsTable = new QTableWidget;
    courseStatsTable->setColumnCount(2);
    courseStatsTable->setHorizontalHeaderLabels({ "课程", "任务数量" });
    courseStatsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    courseStatsTable->horizontalHeader()->setStretchLastSection(true);
    courseStatsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    UiTheme::polishTable(courseStatsTable);

    courseLayout->addWidget(courseTitle);
    courseLayout->addWidget(courseHint);
    courseLayout->addWidget(courseStatsTable, 1);

    QFrame* helperCard = UiTheme::createCard();
    helperCard->setFixedWidth(280);

    QVBoxLayout* helperLayout = new QVBoxLayout(helperCard);
    helperLayout->setContentsMargins(20, 20, 20, 20);
    helperLayout->setSpacing(12);

    QLabel* helperTitle = new QLabel("辅助解读");
    helperTitle->setProperty("role", "sectionTitle");

    QLabel* helperText = new QLabel("统计页更适合回看结果，不建议在这里处理具体任务。若发现某门课程任务数量异常多，可以返回任务页进一步筛选。");
    helperText->setProperty("role", "muted");
    helperText->setWordWrap(true);

    QLabel* helperPointOne = new QLabel("1. 完成率越低，越需要回到计划页重排节奏。");
    helperPointOne->setProperty("role", "muted");
    helperPointOne->setWordWrap(true);

    QLabel* helperPointTwo = new QLabel("2. 若某门课程任务过多，说明需要更早拆分任务。");
    helperPointTwo->setProperty("role", "muted");
    helperPointTwo->setWordWrap(true);

    QLabel* helperPointThree = new QLabel("3. 统计数据会随课程和任务变更自动刷新。");
    helperPointThree->setProperty("role", "muted");
    helperPointThree->setWordWrap(true);

    helperLayout->addWidget(helperTitle);
    helperLayout->addWidget(helperText);
    helperLayout->addWidget(helperPointOne);
    helperLayout->addWidget(helperPointTwo);
    helperLayout->addWidget(helperPointThree);
    helperLayout->addStretch();

    QVBoxLayout* mainContentLayout = new QVBoxLayout;
    mainContentLayout->setSpacing(18);
    mainContentLayout->addWidget(progressCard);
    mainContentLayout->addWidget(courseCard, 1);

    QHBoxLayout* bodyLayout = new QHBoxLayout;
    bodyLayout->setSpacing(18);
    bodyLayout->addLayout(mainContentLayout, 1);
    bodyLayout->addWidget(helperCard);

    pageLayout->addWidget(introCard);
    pageLayout->addLayout(bodyLayout, 1);
}

void StatsPage::refresh()
{
    const QVariantMap stats = manager->getStatistics();
    const int totalTasks = stats["totalTasks"].toInt();
    const int completedTasks = stats["completedTasks"].toInt();
    const int unfinishedTasks = stats["unfinishedTasks"].toInt();
    const int weekCompletedTasks = stats["weekCompletedTasks"].toInt();
    const int weekTotalTasks = stats["weekTotalTasks"].toInt();
    const int completionRate = stats["completionRate"].toInt();

    totalLabel->setText(QString::number(totalTasks));
    completedLabel->setText(QString::number(completedTasks));
    unfinishedLabel->setText(QString::number(unfinishedTasks));
    weekLabel->setText(QString("%1%").arg(completionRate));
    completionProgress->setValue(completionRate);
    completionProgress->setToolTip(QString("本周完成 %1 / %2").arg(weekCompletedTasks).arg(weekTotalTasks));

    const QVariantMap courseTaskCount = stats["courseTaskCount"].toMap();
    courseStatsTable->setRowCount(courseTaskCount.size());
    int row = 0;
    for (auto it = courseTaskCount.constBegin(); it != courseTaskCount.constEnd(); ++it) {
        courseStatsTable->setItem(row, 0, new QTableWidgetItem(it.key()));
        courseStatsTable->setItem(row, 1, new QTableWidgetItem(QString::number(it.value().toInt())));
        ++row;
    }
    courseStatsTable->resizeColumnsToContents();
}
