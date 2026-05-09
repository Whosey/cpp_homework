#ifndef COURSEPAGE_H
#define COURSEPAGE_H

#include "../managers/SystemManager.h"

#include <QTableWidget>
#include <QWidget>

class CoursePage : public QWidget
{
    Q_OBJECT

public:
    explicit CoursePage(SystemManager* manager, QWidget* parent = nullptr);
    void refresh();

signals:
    void dataChanged();

private slots:
    void addCourse();
    void editCourse();
    void deleteCourse();

private:
    SystemManager* manager;
    QTableWidget* table;

    QString selectedCourseId() const;
    bool showCourseDialog(const QString& title, QString& courseName, QString& teacherName, QString& note);
};

#endif
