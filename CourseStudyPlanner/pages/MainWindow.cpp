#include "MainWindow.h"

#include "UiTheme.h"

#include <QDate>
#include <QDialog>
#include <QFormLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QScrollArea>
#include <QVBoxLayout>

MainWindow::MainWindow(SystemManager* manager, QWidget* parent)
    : QMainWindow(parent), manager(manager)
{
    setWindowTitle("课程作业与学习规划系统");

    QWidget* central = new QWidget;
    setCentralWidget(central);

    QHBoxLayout* shellLayout = new QHBoxLayout(central);
    shellLayout->setContentsMargins(24, 24, 24, 24);
    shellLayout->setSpacing(20);

    QFrame* sidebar = new QFrame;
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(248);

    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(22, 26, 22, 22);
    sidebarLayout->setSpacing(16);

    QLabel* brandTag = new QLabel("学习工作台");
    brandTag->setProperty("role", "eyebrow");
    brandTag->setStyleSheet("color: #CBD5E1;");

    QLabel* brandTitle = new QLabel("课程、任务、计划\n统一管理");
    brandTitle->setProperty("role", "hero");
    brandTitle->setWordWrap(true);

    QLabel* brandDescription = new QLabel("左侧是主操作区，中间是当前工作内容，右侧保留概览与提示，让日常学习安排更像一个产品化工作台。");
    brandDescription->setProperty("role", "sidebarMuted");
    brandDescription->setWordWrap(true);

    QFrame* profileCard = UiTheme::createCard();
    QVBoxLayout* profileLayout = new QVBoxLayout(profileCard);
    profileLayout->setContentsMargins(16, 16, 16, 16);
    profileLayout->setSpacing(6);

    QLabel* profileTitle = new QLabel("当前账号");
    profileTitle->setProperty("role", "eyebrow");

    welcomeLabel = new QLabel(manager->getCurrentUsername());
    welcomeLabel->setProperty("role", "sidebarTitle");
    welcomeLabel->setStyleSheet("background: transparent;");

    QLabel* profileSubtitle = new QLabel(QDate::currentDate().toString("yyyy年MM月dd日"));
    profileSubtitle->setProperty("role", "muted");

    profileLayout->addWidget(profileTitle);
    profileLayout->addWidget(welcomeLabel);
    profileLayout->addWidget(profileSubtitle);

    QLabel* navTitle = new QLabel("主操作区");
    navTitle->setProperty("role", "eyebrow");

    courseButton = createNavButton("课程管理");
    taskButton = createNavButton("任务管理");
    planButton = createNavButton("学习计划");
    statsButton = createNavButton("统计分析");
    passwordButton = createNavButton("修改密码");
    logoutButton = createNavButton("退出登录");

    sidebarLayout->addWidget(brandTag);
    sidebarLayout->addWidget(brandTitle);
    sidebarLayout->addWidget(brandDescription);
    sidebarLayout->addWidget(profileCard);
    sidebarLayout->addSpacing(4);
    sidebarLayout->addWidget(navTitle);
    sidebarLayout->addWidget(courseButton);
    sidebarLayout->addWidget(taskButton);
    sidebarLayout->addWidget(planButton);
    sidebarLayout->addWidget(statsButton);
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(passwordButton);
    sidebarLayout->addWidget(logoutButton);

    QWidget* contentArea = new QWidget;
    QVBoxLayout* contentLayout = new QVBoxLayout(contentArea);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(18);

    QFrame* topBar = UiTheme::createCard();
    QHBoxLayout* topBarLayout = new QHBoxLayout(topBar);
    topBarLayout->setContentsMargins(24, 22, 24, 22);
    topBarLayout->setSpacing(18);

    QWidget* headerTextContainer = new QWidget;
    QVBoxLayout* headerTextLayout = new QVBoxLayout(headerTextContainer);
    headerTextLayout->setContentsMargins(0, 0, 0, 0);
    headerTextLayout->setSpacing(6);

    pageEyebrowLabel = new QLabel("学习工作台");
    pageEyebrowLabel->setProperty("role", "eyebrow");

    pageTitleLabel = new QLabel("课程管理");
    pageTitleLabel->setProperty("role", "pageTitle");
    pageTitleLabel->setWordWrap(true);

    pageSubtitleLabel = new QLabel("统一维护课程、作业、计划和进度，减少页面之间来回切换。");
    pageSubtitleLabel->setProperty("role", "muted");
    pageSubtitleLabel->setWordWrap(true);

    headerTextLayout->addWidget(pageEyebrowLabel);
    headerTextLayout->addWidget(pageTitleLabel);
    headerTextLayout->addWidget(pageSubtitleLabel);

    QFrame* statusCard = UiTheme::createCard();
    QVBoxLayout* statusLayout = new QVBoxLayout(statusCard);
    statusLayout->setContentsMargins(18, 16, 18, 16);
    statusLayout->setSpacing(4);

    QLabel* statusEyebrow = new QLabel("当前模式");
    statusEyebrow->setProperty("role", "eyebrow");

    sectionNameLabel = new QLabel("课程管理");
    sectionNameLabel->setProperty("role", "cardTitle");

    sectionHintLabel = new QLabel("先维护课程，再安排关联任务。");
    sectionHintLabel->setProperty("role", "muted");
    sectionHintLabel->setWordWrap(true);

    statusLayout->addWidget(statusEyebrow);
    statusLayout->addWidget(sectionNameLabel);
    statusLayout->addWidget(sectionHintLabel);

    topBarLayout->addWidget(headerTextContainer, 1);
    topBarLayout->addWidget(statusCard, 0);

    coursePage = new CoursePage(manager);
    taskPage = new TaskPage(manager);
    planPage = new PlanPage(manager);
    statsPage = new StatsPage(manager);

    stackedWidget = new QStackedWidget;
    stackedWidget->setStyleSheet("QStackedWidget { background: transparent; }");
    stackedWidget->addWidget(coursePage);
    stackedWidget->addWidget(taskPage);
    stackedWidget->addWidget(planPage);
    stackedWidget->addWidget(statsPage);

    QScrollArea* pageScrollArea = new QScrollArea;
    pageScrollArea->setFrameShape(QFrame::NoFrame);
    pageScrollArea->setWidgetResizable(true);
    pageScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pageScrollArea->setWidget(stackedWidget);

    contentLayout->addWidget(topBar);
    contentLayout->addWidget(pageScrollArea, 1);

    QFrame* helperPanel = UiTheme::createCard();
    helperPanel->setFixedWidth(290);

    QVBoxLayout* helperLayout = new QVBoxLayout(helperPanel);
    helperLayout->setContentsMargins(20, 22, 20, 22);
    helperLayout->setSpacing(16);

    QLabel* helperEyebrow = new QLabel("辅助区");
    helperEyebrow->setProperty("role", "eyebrow");

    QLabel* helperTitle = new QLabel("今日概览");
    helperTitle->setProperty("role", "sectionTitle");

    QLabel* helperSubtitle = new QLabel("在切换页面时，右侧始终保留当前工作状态和操作提示。");
    helperSubtitle->setProperty("role", "muted");
    helperSubtitle->setWordWrap(true);

    QFrame* courseCard = UiTheme::createCard();
    QVBoxLayout* courseCardLayout = new QVBoxLayout(courseCard);
    courseCardLayout->setContentsMargins(16, 16, 16, 16);
    courseCardLayout->setSpacing(6);
    courseOverviewLabel = new QLabel("0");
    courseOverviewLabel->setProperty("role", "metricValue");
    QLabel* courseCaption = new QLabel("课程总数");
    courseCaption->setProperty("role", "metricCaption");
    courseCardLayout->addWidget(courseOverviewLabel);
    courseCardLayout->addWidget(courseCaption);

    QFrame* taskCard = UiTheme::createCard();
    QVBoxLayout* taskCardLayout = new QVBoxLayout(taskCard);
    taskCardLayout->setContentsMargins(16, 16, 16, 16);
    taskCardLayout->setSpacing(6);
    taskOverviewLabel = new QLabel("0");
    taskOverviewLabel->setProperty("role", "metricValue");
    QLabel* taskCaption = new QLabel("任务总数");
    taskCaption->setProperty("role", "metricCaption");
    taskCardLayout->addWidget(taskOverviewLabel);
    taskCardLayout->addWidget(taskCaption);

    QFrame* planCard = UiTheme::createCard();
    QVBoxLayout* planCardLayout = new QVBoxLayout(planCard);
    planCardLayout->setContentsMargins(16, 16, 16, 16);
    planCardLayout->setSpacing(6);
    planOverviewLabel = new QLabel("0");
    planOverviewLabel->setProperty("role", "metricValue");
    QLabel* planCaption = new QLabel("今日计划");
    planCaption->setProperty("role", "metricCaption");
    planCardLayout->addWidget(planOverviewLabel);
    planCardLayout->addWidget(planCaption);

    QFrame* tipsCard = UiTheme::createCard();
    QVBoxLayout* tipsLayout = new QVBoxLayout(tipsCard);
    tipsLayout->setContentsMargins(16, 16, 16, 16);
    tipsLayout->setSpacing(10);

    QLabel* tipsTitle = new QLabel("操作建议");
    tipsTitle->setProperty("role", "cardTitle");

    QLabel* tipOne = new QLabel("1. 先建立课程，再给任务绑定课程归属。");
    tipOne->setProperty("role", "muted");
    tipOne->setWordWrap(true);

    QLabel* tipTwo = new QLabel("2. 在任务页集中做新增、筛选和完成切换。");
    tipTwo->setProperty("role", "muted");
    tipTwo->setWordWrap(true);

    QLabel* tipThree = new QLabel("3. 学习计划页适合查看今天与本周的执行重点。");
    tipThree->setProperty("role", "muted");
    tipThree->setWordWrap(true);

    tipsLayout->addWidget(tipsTitle);
    tipsLayout->addWidget(tipOne);
    tipsLayout->addWidget(tipTwo);
    tipsLayout->addWidget(tipThree);

    helperLayout->addWidget(helperEyebrow);
    helperLayout->addWidget(helperTitle);
    helperLayout->addWidget(helperSubtitle);
    helperLayout->addWidget(courseCard);
    helperLayout->addWidget(taskCard);
    helperLayout->addWidget(planCard);
    helperLayout->addWidget(tipsCard);
    helperLayout->addStretch();

    shellLayout->addWidget(sidebar);
    shellLayout->addWidget(contentArea, 1);
    shellLayout->addWidget(helperPanel);

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
    button->setCheckable(true);
    button->setMinimumHeight(48);
    UiTheme::setButtonVariant(button, "nav");
    return button;
}

void MainWindow::setActiveNavButton(QPushButton* activeButton)
{
    for (QPushButton* button : { courseButton, taskButton, planButton, statsButton, passwordButton, logoutButton }) {
        if (button) {
            button->setChecked(button == activeButton);
        }
    }
}

void MainWindow::showCoursePage()
{
    coursePage->refresh();
    stackedWidget->setCurrentWidget(coursePage);
    setActiveNavButton(courseButton);
    updatePageHeader("课程工作台", "课程管理", "在这里维护课程信息，为任务分配和统计分析建立清晰的归属关系。");
    refreshOverviewPanel("课程管理", "当前重点是补齐课程名称、任课教师和备注信息。");
}

void MainWindow::showTaskPage()
{
    taskPage->refresh();
    stackedWidget->setCurrentWidget(taskPage);
    setActiveNavButton(taskButton);
    updatePageHeader("执行中心", "任务管理", "把新增、筛选、编辑和完成切换都集中在一个主界面里，减少反复跳转。");
    refreshOverviewPanel("任务管理", "建议优先处理高优先级和截止日期临近的任务。");
}

void MainWindow::showPlanPage()
{
    planPage->refresh();
    stackedWidget->setCurrentWidget(planPage);
    setActiveNavButton(planButton);
    updatePageHeader("计划视图", "学习计划", "把今天、本周和提醒信息拆开呈现，让安排节奏更清楚。");
    refreshOverviewPanel("学习计划", "先看今日任务，再根据提醒列表安排本周节奏。");
}

void MainWindow::showStatsPage()
{
    statsPage->refresh();
    stackedWidget->setCurrentWidget(statsPage);
    setActiveNavButton(statsButton);
    updatePageHeader("结果回看", "统计分析", "从完成率、课程任务分布和本周进度三个角度回看学习状态。");
    refreshOverviewPanel("统计分析", "适合用来回看本周执行情况和课程负载分布。");
}

void MainWindow::changePassword()
{
    QDialog dialog(this);
    dialog.setWindowTitle("修改密码");
    dialog.resize(420, 240);

    QVBoxLayout* rootLayout = new QVBoxLayout(&dialog);
    rootLayout->setContentsMargins(24, 24, 24, 24);
    rootLayout->setSpacing(18);

    QLabel* title = new QLabel("修改当前账号密码");
    title->setProperty("role", "sectionTitle");

    QLabel* subtitle = new QLabel("请输入旧密码和新密码，修改只影响当前登录账号。");
    subtitle->setProperty("role", "muted");
    subtitle->setWordWrap(true);

    QLineEdit* oldPasswordEdit = new QLineEdit;
    oldPasswordEdit->setEchoMode(QLineEdit::Password);
    oldPasswordEdit->setPlaceholderText("请输入旧密码");

    QLineEdit* newPasswordEdit = new QLineEdit;
    newPasswordEdit->setEchoMode(QLineEdit::Password);
    newPasswordEdit->setPlaceholderText("请输入新密码");

    QFormLayout* formLayout = new QFormLayout;
    formLayout->setLabelAlignment(Qt::AlignLeft);
    formLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    formLayout->setHorizontalSpacing(16);
    formLayout->setVerticalSpacing(14);
    formLayout->addRow("旧密码", oldPasswordEdit);
    formLayout->addRow("新密码", newPasswordEdit);

    QPushButton* cancelButton = new QPushButton("取消");
    QPushButton* okButton = new QPushButton("保存密码");
    UiTheme::setButtonVariant(okButton, "primary");

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(okButton);

    rootLayout->addWidget(title);
    rootLayout->addWidget(subtitle);
    rootLayout->addLayout(formLayout);
    rootLayout->addStretch();
    rootLayout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString errorMessage;
        if (!manager->changePassword(oldPasswordEdit->text(), newPasswordEdit->text(), errorMessage)) {
            QMessageBox::warning(this, "修改失败", errorMessage);
            return;
        }
        QMessageBox::information(this, "修改成功", "密码已更新。");
    }
}

void MainWindow::refreshAllPages()
{
    welcomeLabel->setText(manager->getCurrentUsername());
    coursePage->refresh();
    taskPage->refresh();
    planPage->refresh();
    statsPage->refresh();
    refreshOverviewPanel(sectionNameLabel->text(), sectionHintLabel->text());
}

void MainWindow::updatePageHeader(const QString& eyebrow, const QString& title, const QString& subtitle)
{
    pageEyebrowLabel->setText(eyebrow);
    pageTitleLabel->setText(title);
    pageSubtitleLabel->setText(subtitle);
}

void MainWindow::refreshOverviewPanel(const QString& sectionName, const QString& sectionHint)
{
    sectionNameLabel->setText(sectionName);
    sectionHintLabel->setText(sectionHint);
    courseOverviewLabel->setText(QString::number(manager->getCurrentUserCourses().size()));
    taskOverviewLabel->setText(QString::number(manager->getCurrentUserTasks().size()));
    planOverviewLabel->setText(QString::number(manager->getTodayPlan().size()));
}
