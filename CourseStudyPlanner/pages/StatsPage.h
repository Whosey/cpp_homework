#ifndef STATSPAGE_H
#define STATSPAGE_H

#include "../managers/SystemManager.h"

#include <QLabel>
#include <QProgressBar>
#include <QTableWidget>
#include <QWidget>

class StatsPage : public QWidget
{
    Q_OBJECT

public:
    explicit StatsPage(SystemManager* manager, QWidget* parent = nullptr);
    void refresh();

private:
    SystemManager* manager;
    QLabel* totalLabel;
    QLabel* completedLabel;
    QLabel* unfinishedLabel;
    QLabel* weekLabel;
    QProgressBar* completionProgress;
    QTableWidget* courseStatsTable;
};

#endif
