#include "managers/SystemManager.h"
#include "pages/LoginPage.h"
#include "pages/MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Course Assignment and Study Planning Management System");

    SystemManager manager;
    LoginPage loginPage(&manager);
    loginPage.resize(420, 300);
    loginPage.show();

    MainWindow* mainWindow = nullptr;

    QObject::connect(&loginPage, &LoginPage::loginSucceeded, [&]() {
        mainWindow = new MainWindow(&manager);
        mainWindow->resize(1050, 680);
        mainWindow->show();
        loginPage.hide();

        QObject::connect(mainWindow, &MainWindow::logoutRequested, [&]() {
            manager.logout();
            mainWindow->close();
            mainWindow->deleteLater();
            mainWindow = nullptr;
            loginPage.clearInputs();
            loginPage.show();
        });
    });

    return app.exec();
}
