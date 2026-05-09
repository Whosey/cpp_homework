#include "LoginPage.h"

#include <QFormLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

LoginPage::LoginPage(SystemManager* manager, QWidget* parent)
    : QWidget(parent), manager(manager)
{
    setWindowTitle("登录 / 注册");

    QLabel* titleLabel = new QLabel("课程作业与学习规划管理系统");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 22px; font-weight: bold; margin: 16px;");

    usernameEdit = new QLineEdit;
    usernameEdit->setPlaceholderText("请输入用户名");

    passwordEdit = new QLineEdit;
    passwordEdit->setPlaceholderText("请输入密码");
    passwordEdit->setEchoMode(QLineEdit::Password);

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("用户名：", usernameEdit);
    formLayout->addRow("密码：", passwordEdit);

    QPushButton* loginButton = new QPushButton("登录");
    QPushButton* registerButton = new QPushButton("注册");

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
    mainLayout->setContentsMargins(40, 30, 40, 30);

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
    QMessageBox::information(this, "注册成功", "账号注册成功，请使用该账号登录。");
}
