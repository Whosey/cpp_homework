#include "MainWindow.h"

#include <QDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(SystemManager* manager, QWidget* parent)
    : QMainWindow(parent), manager(manager)
{
    setWindowTitle("Course Assignment and Study Planning Management System");

    QWidget* central = new QWidget;
    setCentralWidget(central);

    welcomeLabel = new QLabel("欢迎，" + manager->getCurrentUsername());
    welcomeLabel->setStyleSheet("font-size: 18px; font-weight: bold; padding: 8px;");

    QPushButton* courseButton = createNavButton("课程管理");
    QPushButton* taskButton = createNavButton("任务管理");
    QPushButton* planButton = createNavButton("学习计划");
    QPushButton* statsButton = createNavButton("统计信息");
    QPushButton* passwordButton = createNavButton("修改密码");
    QPushButton* logoutButton = createNavButton("退出登录");

    QVBoxLayout* navLayout = new QVBoxLayout;
    navLayout->addWidget(welcomeLabel);
    navLayout->addWidget(courseButton);
    navLayout->addWidget(taskButton);
    navLayout->addWidget(planButton);
    navLayout->addWidget(statsButton);
    navLayout->addSpacing(16);
    navLayout->addWidget(passwordButton);
    navLayout->addStretch();
    navLayout->addWidget(logoutButton);

    QWidget* navWidget = new QWidget;
    navWidget->setLayout(navLayout);
    navWidget->setFixedWidth(180);
    navWidget->setStyleSheet("background:#f3f5f7;");

    coursePage = new CoursePage(manager);
    taskPage = new TaskPage(manager);
    planPage = new PlanPage(manager);
    statsPage = new StatsPage(manager);

    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(coursePage);
    stackedWidget->addWidget(taskPage);
    stackedWidget->addWidget(planPage);
    stackedWidget->addWidget(statsPage);

    QHBoxLayout* mainLayout = new QHBoxLayout(central);
    mainLayout->addWidget(navWidget);
    mainLayout->addWidget(stackedWidget, 1);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    connect(courseButton, &QPushButton::clicked, this, &MainWindow::showCoursePage);
    connect(taskButton, &QPushButton::clicked, this, &MainWindow::showTaskPage);
    connect(planButton, &QPushButton::clicked, this, &MainWindow::showPlanPage);
    connect(statsButton, &QPushButton::clicked, this, &MainWindow::showStatsPage);
    connect(passwordButton, &QPushButton::clicked, this, &MainWindow::changePassword);
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::logoutRequested);

    connect(coursePage, &CoursePage::dataChanged, this, &MainWindow::refreshAllPages);
    connect(taskPage, &TaskPage::dataChanged, this, &MainWindow::refreshAllPages);

    showCoursePage();
}

QPushButton* MainWindow::createNavButton(const QString& text)
{
    QPushButton* button = new QPushButton(text);
    button->setMinimumHeight(38);
    button->setStyleSheet("QPushButton{text-align:left;padding-left:16px;} QPushButton:hover{background:#e4edf7;}");
    return button;
}

void MainWindow::showCoursePage()
{
    coursePage->refresh();
    stackedWidget->setCurrentWidget(coursePage);
}

void MainWindow::showTaskPage()
{
    taskPage->refresh();
    stackedWidget->setCurrentWidget(taskPage);
}

void MainWindow::showPlanPage()
{
    planPage->refresh();
    stackedWidget->setCurrentWidget(planPage);
}

void MainWindow::showStatsPage()
{
    statsPage->refresh();
    stackedWidget->setCurrentWidget(statsPage);
}

void MainWindow::changePassword()
{
    QDialog dialog(this);
    dialog.setWindowTitle("修改密码");

    QLineEdit* oldPasswordEdit = new QLineEdit;
    QLineEdit* newPasswordEdit = new QLineEdit;
    oldPasswordEdit->setEchoMode(QLineEdit::Password);
    newPasswordEdit->setEchoMode(QLineEdit::Password);

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("原密码：", oldPasswordEdit);
    formLayout->addRow("新密码：", newPasswordEdit);

    QPushButton* okButton = new QPushButton("确认");
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

    if (dialog.exec() == QDialog::Accepted) {
        QString errorMessage;
        if (!manager->changePassword(oldPasswordEdit->text(), newPasswordEdit->text(), errorMessage)) {
            QMessageBox::warning(this, "修改失败", errorMessage);
            return;
        }
        QMessageBox::information(this, "修改成功", "密码已修改。");
    }
}

void MainWindow::refreshAllPages()
{
    coursePage->refresh();
    taskPage->refresh();
    planPage->refresh();
    statsPage->refresh();
}
