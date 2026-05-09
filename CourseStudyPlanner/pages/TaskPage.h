#ifndef TASKPAGE_H
#define TASKPAGE_H

#include "../managers/SystemManager.h"

#include <QComboBox>
#include <QTableWidget>
#include <QWidget>

class TaskPage : public QWidget
{
    Q_OBJECT

public:
    explicit TaskPage(SystemManager* manager, QWidget* parent = nullptr);
    void refresh();

signals:
    void dataChanged();

private slots:
    void addTask();
    void editTask();
    void deleteTask();
    void toggleTaskCompleted();

private:
    SystemManager* manager;
    QTableWidget* table;
    QComboBox* courseFilterCombo;
    QComboBox* typeFilterCombo;
    QComboBox* stateFilterCombo;
    QComboBox* sortCombo;

    QString selectedTaskId() const;
    BaseTask* findTaskById(const QString& taskId) const;
    void refreshCourseFilter();
    bool showTaskDialog(const QString& title,
                        BaseTask* task,
                        QString& taskTitle,
                        QString& description,
                        QDate& deadline,
                        int& priority,
                        QString& relatedCourseId,
                        QString& taskType,
                        bool& isCompleted);
};

#endif
