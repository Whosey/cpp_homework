#include "LoginPage.h"

#include "UiTheme.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

LoginPage::LoginPage(SystemManager* manager, QWidget* parent)
    : QWidget(parent), manager(manager)
{
    setWindowTitle("课程作业与学习规划系统");

    QHBoxLayout* rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(28, 28, 28, 28);
    rootLayout->setSpacing(20);

    QFrame* heroPanel = new QFrame;
    heroPanel->setObjectName("sidebar");
    heroPanel->setMinimumWidth(360);

    QVBoxLayout* heroLayout = new QVBoxLayout(heroPanel);
    heroLayout->setContentsMargins(30, 32, 30, 32);
    heroLayout->setSpacing(18);

    QLabel* heroEyebrow = new QLabel("学习入口");
    heroEyebrow->setProperty("role", "eyebrow");
    heroEyebrow->setStyleSheet("color: #CBD5E1;");

    QLabel* heroTitle = new QLabel("课程、任务、截止时间\n一眼看清");
    heroTitle->setProperty("role", "hero");
    heroTitle->setWordWrap(true);

    QLabel* heroDescription = new QLabel("登录后，你会进入一个三层结构的学习面板：左边做主操作，中间处理核心内容，右边随时查看提示和概览。");
    heroDescription->setProperty("role", "sidebarMuted");
    heroDescription->setWordWrap(true);

    QFrame* pointOneCard = UiTheme::createCard();
    QVBoxLayout* pointOneLayout = new QVBoxLayout(pointOneCard);
    pointOneLayout->setContentsMargins(16, 16, 16, 16);
    pointOneLayout->setSpacing(4);
    QLabel* pointOneTag = new QLabel("01");
    pointOneTag->setProperty("role", "accent");
    QLabel* pointOne = new QLabel("先建立课程目录，所有任务会有明确归属。");
    pointOne->setProperty("role", "highlight");
    pointOne->setWordWrap(true);
    pointOneLayout->addWidget(pointOneTag);
    pointOneLayout->addWidget(pointOne);

    QFrame* pointTwoCard = UiTheme::createCard();
    QVBoxLayout* pointTwoLayout = new QVBoxLayout(pointTwoCard);
    pointTwoLayout->setContentsMargins(16, 16, 16, 16);
    pointTwoLayout->setSpacing(4);
    QLabel* pointTwoTag = new QLabel("02");
    pointTwoTag->setProperty("role", "accent");
    QLabel* pointTwo = new QLabel("任务页集中处理筛选、编辑、完成切换。");
    pointTwo->setProperty("role", "highlight");
    pointTwo->setWordWrap(true);
    pointTwoLayout->addWidget(pointTwoTag);
    pointTwoLayout->addWidget(pointTwo);

    QFrame* pointThreeCard = UiTheme::createCard();
    QVBoxLayout* pointThreeLayout = new QVBoxLayout(pointThreeCard);
    pointThreeLayout->setContentsMargins(16, 16, 16, 16);
    pointThreeLayout->setSpacing(4);
    QLabel* pointThreeTag = new QLabel("03");
    pointThreeTag->setProperty("role", "accent");
    QLabel* pointThree = new QLabel("计划与统计页帮助你回看本周执行状态。");
    pointThree->setProperty("role", "highlight");
    pointThree->setWordWrap(true);
    pointThreeLayout->addWidget(pointThreeTag);
    pointThreeLayout->addWidget(pointThree);

    heroLayout->addWidget(heroEyebrow);
    heroLayout->addWidget(heroTitle);
    heroLayout->addWidget(heroDescription);
    heroLayout->addSpacing(10);
    heroLayout->addWidget(pointOneCard);
    heroLayout->addWidget(pointTwoCard);
    heroLayout->addWidget(pointThreeCard);
    heroLayout->addStretch();

    QFrame* authCard = UiTheme::createCard();
    authCard->setMinimumWidth(360);

    QVBoxLayout* authLayout = new QVBoxLayout(authCard);
    authLayout->setContentsMargins(34, 34, 34, 34);
    authLayout->setSpacing(18);

    QLabel* formEyebrow = new QLabel("主操作区");
    formEyebrow->setProperty("role", "eyebrow");

    QLabel* titleLabel = new QLabel("登录学习工作台");
    titleLabel->setProperty("role", "pageTitle");
    titleLabel->setWordWrap(true);

    QLabel* subtitleLabel = new QLabel("输入账号密码即可进入系统；如果还没有账号，也可以直接在这里完成注册。");
    subtitleLabel->setProperty("role", "muted");
    subtitleLabel->setWordWrap(true);

    QLabel* usernameLabel = new QLabel("用户名");
    usernameLabel->setProperty("role", "cardTitle");
    usernameLabel->setStyleSheet("font-size: 13px;");

    usernameEdit = new QLineEdit;
    usernameEdit->setPlaceholderText("请输入用户名");

    QLabel* passwordLabel = new QLabel("密码");
    passwordLabel->setProperty("role", "cardTitle");
    passwordLabel->setStyleSheet("font-size: 13px;");

    passwordEdit = new QLineEdit;
    passwordEdit->setPlaceholderText("请输入密码");
    passwordEdit->setEchoMode(QLineEdit::Password);

    QPushButton* loginButton = new QPushButton("登录");
    UiTheme::setButtonVariant(loginButton, "primary");

    QPushButton* registerButton = new QPushButton("注册账号");
    UiTheme::setButtonVariant(registerButton, "secondary");

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(12);
    buttonLayout->addWidget(loginButton, 1);
    buttonLayout->addWidget(registerButton, 1);

    authLayout->addWidget(formEyebrow);
    authLayout->addWidget(titleLabel);
    authLayout->addWidget(subtitleLabel);
    authLayout->addSpacing(8);
    authLayout->addWidget(usernameLabel);
    authLayout->addWidget(usernameEdit);
    authLayout->addWidget(passwordLabel);
    authLayout->addWidget(passwordEdit);
    authLayout->addSpacing(6);
    authLayout->addLayout(buttonLayout);
    authLayout->addStretch();

    QFrame* helperCard = UiTheme::createCard();
    helperCard->setMinimumWidth(280);

    QVBoxLayout* helperLayout = new QVBoxLayout(helperCard);
    helperLayout->setContentsMargins(24, 24, 24, 24);
    helperLayout->setSpacing(16);

    QLabel* helperEyebrow = new QLabel("辅助区");
    helperEyebrow->setProperty("role", "eyebrow");

    QLabel* helperTitle = new QLabel("开始前建议");
    helperTitle->setProperty("role", "sectionTitle");

    QLabel* helperText = new QLabel("首次使用建议先注册账号，然后按“课程管理 → 任务管理 → 学习计划 → 统计分析”的顺序完成一次完整体验。");
    helperText->setProperty("role", "muted");
    helperText->setWordWrap(true);

    QLabel* helperPointOne = new QLabel("1. 课程信息支持课程名、教师名和备注。");
    helperPointOne->setProperty("role", "muted");
    helperPointOne->setWordWrap(true);

    QLabel* helperPointTwo = new QLabel("2. 任务支持按课程、类型、状态和排序方式筛选。");
    helperPointTwo->setProperty("role", "muted");
    helperPointTwo->setWordWrap(true);

    QLabel* helperPointThree = new QLabel("3. 统计页会自动汇总完成率和课程任务分布。");
    helperPointThree->setProperty("role", "muted");
    helperPointThree->setWordWrap(true);

    helperLayout->addWidget(helperEyebrow);
    helperLayout->addWidget(helperTitle);
    helperLayout->addWidget(helperText);
    helperLayout->addWidget(helperPointOne);
    helperLayout->addWidget(helperPointTwo);
    helperLayout->addWidget(helperPointThree);
    helperLayout->addStretch();

    rootLayout->addWidget(heroPanel, 4);
    rootLayout->addWidget(authCard, 4);
    rootLayout->addWidget(helperCard, 3);

    connect(loginButton, &QPushButton::clicked, this, &LoginPage::handleLogin);
    connect(registerButton, &QPushButton::clicked, this, &LoginPage::handleRegister);
}

void LoginPage::clearInputs()
{
    passwordEdit->clear();
}

void LoginPage::handleLogin()
{
    QString errorMessage;
    if (!manager->login(usernameEdit->text(), passwordEdit->text(), errorMessage)) {
        QMessageBox::warning(this, "登录失败", errorMessage);
        return;
    }
    emit loginSucceeded();
}

void LoginPage::handleRegister()
{
    QString errorMessage;
    if (!manager->registerUser(usernameEdit->text(), passwordEdit->text(), errorMessage)) {
        QMessageBox::warning(this, "注册失败", errorMessage);
        return;
    }
    QMessageBox::information(this, "注册成功", "账号已创建，请使用同一组用户名和密码登录。");
}
