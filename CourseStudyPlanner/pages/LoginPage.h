#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include "../managers/SystemManager.h"

#include <QLineEdit>
#include <QWidget>

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(SystemManager* manager, QWidget* parent = nullptr);
    void clearInputs();

signals:
    void loginSucceeded();

private slots:
    void handleLogin();
    void handleRegister();

private:
    SystemManager* manager;
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
};

#endif
