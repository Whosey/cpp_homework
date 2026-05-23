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
    QLabel* pageEyebrowLabel;
    QLabel* pageTitleLabel;
    QLabel* pageSubtitleLabel;
    QLabel* sectionNameLabel;
    QLabel* sectionHintLabel;
    QLabel* courseOverviewLabel;
    QLabel* taskOverviewLabel;
    QLabel* planOverviewLabel;
    QStackedWidget* stackedWidget;
    CoursePage* coursePage;
    TaskPage* taskPage;
    PlanPage* planPage;
    StatsPage* statsPage;
    QPushButton* courseButton;
    QPushButton* taskButton;
    QPushButton* planButton;
    QPushButton* statsButton;
    QPushButton* passwordButton;
    QPushButton* logoutButton;

    QPushButton* createNavButton(const QString& text);
    void setActiveNavButton(QPushButton* activeButton);
    void updatePageHeader(const QString& eyebrow, const QString& title, const QString& subtitle);
    void refreshOverviewPanel(const QString& sectionName, const QString& sectionHint);
};

#endif
