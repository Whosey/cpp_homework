#include "StatsPage.h"

#include <QAbstractItemView>
#include <QGroupBox>
#include <QHeaderView>
#include <QVBoxLayout>

StatsPage::StatsPage(SystemManager* manager, QWidget* parent)
    : QWidget(parent), manager(manager)
{
    QLabel* titleLabel = new QLabel("统计信息");
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");

    totalLabel = new QLabel;
    completedLabel = new QLabel;
    unfinishedLabel = new QLabel;
    weekLabel = new QLabel;

    completionProgress = new QProgressBar;
    completionProgress->setRange(0, 100);
    completionProgress->setFormat("本周完成率：%p%");

    QGroupBox* numberBox = new QGroupBox("任务数量");
    QVBoxLayout* numberLayout = new QVBoxLayout(numberBox);
    numberLayout->addWidget(totalLabel);
    numberLayout->addWidget(completedLabel);
    numberLayout->addWidget(unfinishedLabel);
    numberLayout->addWidget(weekLabel);
    numberLayout->addWidget(completionProgress);

    courseStatsTable = new QTableWidget;
    courseStatsTable->setColumnCount(2);
    courseStatsTable->setHorizontalHeaderLabels({ "课程", "任务数量" });
    courseStatsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    courseStatsTable->horizontalHeader()->setStretchLastSection(true);

    QGroupBox* courseBox = new QGroupBox("每门课程任务数量");
    QVBoxLayout* courseLayout = new QVBoxLayout(courseBox);
    courseLayout->addWidget(courseStatsTable);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(numberBox);
    layout->addWidget(courseBox, 1);
}

void StatsPage::refresh()
{
    const QVariantMap stats = manager->getStatistics();
    totalLabel->setText("总任务数量：" + QString::number(stats["totalTasks"].toInt()));
    completedLabel->setText("已完成任务数量：" + QString::number(stats["completedTasks"].toInt()));
    unfinishedLabel->setText("未完成任务数量：" + QString::number(stats["unfinishedTasks"].toInt()));
    weekLabel->setText("本周任务：" + QString::number(stats["weekCompletedTasks"].toInt())
                       + " / " + QString::number(stats["weekTotalTasks"].toInt()));
    completionProgress->setValue(stats["completionRate"].toInt());

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
