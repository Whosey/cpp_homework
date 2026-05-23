#ifndef PLANPAGE_H
#define PLANPAGE_H

#include "../managers/SystemManager.h"

#include <QLabel>
#include <QListWidget>
#include <QTableWidget>
#include <QWidget>

class PlanPage : public QWidget
{
    Q_OBJECT

public:
    explicit PlanPage(SystemManager* manager, QWidget* parent = nullptr);
    void refresh();

private:
    SystemManager* manager;
    QLabel* todayCountLabel;
    QLabel* weekCountLabel;
    QLabel* reminderCountLabel;
    QTableWidget* todayTable;
    QTableWidget* weekTable;
    QListWidget* reminderList;

    void fillTaskTable(QTableWidget* table, const QList<BaseTask*>& tasks);
};

#endif
