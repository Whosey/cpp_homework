#include "managers/SystemManager.h"
#include "pages/LoginPage.h"
#include "pages/MainWindow.h"
#include "pages/UiTheme.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("课程作业与学习规划系统");
    UiTheme::applyApplicationStyle(app);

    SystemManager manager;
    LoginPage loginPage(&manager);
    loginPage.resize(1080, 680);
    loginPage.setMinimumSize(920, 620);
    loginPage.show();

    MainWindow* mainWindow = nullptr;

    QObject::connect(&loginPage, &LoginPage::loginSucceeded, [&]() {
        mainWindow = new MainWindow(&manager);
        mainWindow->resize(1320, 820);
        mainWindow->setMinimumSize(1160, 720);
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
