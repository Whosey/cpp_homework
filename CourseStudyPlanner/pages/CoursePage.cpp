#include "CoursePage.h"

#include <QAbstractItemView>
#include <QDialog>
#include <QFormLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

CoursePage::CoursePage(SystemManager* manager, QWidget* parent)
    : QWidget(parent), manager(manager)
{
    QLabel* titleLabel = new QLabel("课程管理");
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");

    QPushButton* addButton = new QPushButton("添加课程");
    QPushButton* editButton = new QPushButton("修改课程");
    QPushButton* deleteButton = new QPushButton("删除课程");

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();

    table = new QTableWidget;
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({ "课程ID", "课程名称", "教师", "备注", "所属用户" });
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setColumnHidden(0, true);
    table->setColumnHidden(4, true);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addLayout(buttonLayout);
    layout->addWidget(table);

    connect(addButton, &QPushButton::clicked, this, &CoursePage::addCourse);
    connect(editButton, &QPushButton::clicked, this, &CoursePage::editCourse);
    connect(deleteButton, &QPushButton::clicked, this, &CoursePage::deleteCourse);
}

void CoursePage::refresh()
{
    const QList<Course> courses = manager->getCurrentUserCourses();
    table->setRowCount(courses.size());
    for (int row = 0; row < courses.size(); ++row) {
        const Course& course = courses[row];
        table->setItem(row, 0, new QTableWidgetItem(course.getCourseId()));
        table->setItem(row, 1, new QTableWidgetItem(course.getCourseName()));
        table->setItem(row, 2, new QTableWidgetItem(course.getTeacherName()));
        table->setItem(row, 3, new QTableWidgetItem(course.getNote()));
        table->setItem(row, 4, new QTableWidgetItem(course.getOwnerUserId()));
    }
    table->resizeColumnsToContents();
}

QString CoursePage::selectedCourseId() const
{
    const int row = table->currentRow();
    if (row < 0 || !table->item(row, 0)) {
        return "";
    }
    return table->item(row, 0)->text();
}

bool CoursePage::showCourseDialog(const QString& title, QString& courseName, QString& teacherName, QString& note)
{
    QDialog dialog(this);
    dialog.setWindowTitle(title);

    QLineEdit* nameEdit = new QLineEdit(courseName);
    QLineEdit* teacherEdit = new QLineEdit(teacherName);
    QTextEdit* noteEdit = new QTextEdit(note);
    noteEdit->setFixedHeight(90);

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("课程名称：", nameEdit);
    formLayout->addRow("教师姓名：", teacherEdit);
    formLayout->addRow("备注：", noteEdit);

    QPushButton* okButton = new QPushButton("保存");
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

    if (dialog.exec() != QDialog::Accepted) {
        return false;
    }

    courseName = nameEdit->text();
    teacherName = teacherEdit->text();
    note = noteEdit->toPlainText();
    return true;
}

void CoursePage::addCourse()
{
    QString courseName;
    QString teacherName;
    QString note;
    if (!showCourseDialog("添加课程", courseName, teacherName, note)) {
        return;
    }

    QString errorMessage;
    if (!manager->addCourse(courseName, teacherName, note, errorMessage)) {
        QMessageBox::warning(this, "添加失败", errorMessage);
        return;
    }
    refresh();
    emit dataChanged();
}

void CoursePage::editCourse()
{
    const QString courseId = selectedCourseId();
    if (courseId.isEmpty()) {
        QMessageBox::information(this, "提示", "请先选择一门课程。");
        return;
    }

    const int row = table->currentRow();
    QString courseName = table->item(row, 1)->text();
    QString teacherName = table->item(row, 2)->text();
    QString note = table->item(row, 3)->text();

    if (!showCourseDialog("修改课程", courseName, teacherName, note)) {
        return;
    }

    QString errorMessage;
    if (!manager->updateCourse(courseId, courseName, teacherName, note, errorMessage)) {
        QMessageBox::warning(this, "修改失败", errorMessage);
        return;
    }
    refresh();
    emit dataChanged();
}

void CoursePage::deleteCourse()
{
    const QString courseId = selectedCourseId();
    if (courseId.isEmpty()) {
        QMessageBox::information(this, "提示", "请先选择一门课程。");
        return;
    }

    if (QMessageBox::question(this, "确认删除", "确定要删除选中的课程吗？相关任务会取消课程关联。")
        != QMessageBox::Yes) {
        return;
    }

    QString errorMessage;
    if (!manager->deleteCourse(courseId, errorMessage)) {
        QMessageBox::warning(this, "删除失败", errorMessage);
        return;
    }
    refresh();
    emit dataChanged();
}
