#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../managers/SystemManager.h"
#include "CoursePage.h"
#include "PlanPage.h"
#include "StatsPage.h"
#include "TaskPage.h"

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(SystemManager* manager, QWidget* parent = nullptr);

signals:
    void logoutRequested();

private slots:
    void showCoursePage();
    void showTaskPage();
    void showPlanPage();
    void showStatsPage();
    void changePassword();
    void refreshAllPages();

private:
    SystemManager* manager;
    QLabel* welcomeLabel;
    QStackedWidget* stackedWidget;
    CoursePage* coursePage;
    TaskPage* taskPage;
    PlanPage* planPage;
    StatsPage* statsPage;

    QPushButton* createNavButton(const QString& text);
};

#endif
