#include "CoursePage.h"

#include "UiTheme.h"

#include <QAbstractItemView>
#include <QDialog>
#include <QFormLayout>
#include <QFrame>
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
    QVBoxLayout* pageLayout = new QVBoxLayout(this);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->setSpacing(18);
    pageLayout->setSizeConstraint(QLayout::SetMinimumSize);

    QFrame* introCard = UiTheme::createCard();
    QVBoxLayout* introLayout = new QVBoxLayout(introCard);
    introLayout->setContentsMargins(24, 22, 24, 22);
    introLayout->setSpacing(14);

    QLabel* eyebrow = new QLabel("课程工作台");
    eyebrow->setProperty("role", "eyebrow");

    QLabel* titleLabel = new QLabel("先整理课程结构，再承接后续任务与统计");
    titleLabel->setProperty("role", "pageTitle");
    titleLabel->setWordWrap(true);

    QLabel* subtitleLabel = new QLabel("课程页是整个系统的起点。把课程名称、任课教师和备注整理清楚，后续任务关联和统计分析才会更清晰。");
    subtitleLabel->setProperty("role", "muted");
    subtitleLabel->setWordWrap(true);

    summaryLabel = new QLabel;
    summaryLabel->setProperty("role", "metricValue");
    summaryLabel->setStyleSheet("font-size: 28px;");

    QLabel* summaryCaption = new QLabel("当前课程总数");
    summaryCaption->setProperty("role", "metricCaption");

    QVBoxLayout* summaryBlock = new QVBoxLayout;
    summaryBlock->setSpacing(2);
    summaryBlock->addWidget(summaryLabel);
    summaryBlock->addWidget(summaryCaption);

    QPushButton* addButton = new QPushButton("新增课程");
    UiTheme::setButtonVariant(addButton, "primary");

    QPushButton* editButton = new QPushButton("编辑所选课程");
    UiTheme::setButtonVariant(editButton, "secondary");
    QPushButton* deleteButton = new QPushButton("删除所选课程");
    UiTheme::setButtonVariant(deleteButton, "danger");

    QHBoxLayout* actionLayout = new QHBoxLayout;
    actionLayout->setSpacing(12);
    actionLayout->addLayout(summaryBlock);
    actionLayout->addStretch();
    actionLayout->addWidget(editButton);
    actionLayout->addWidget(deleteButton);
    actionLayout->addWidget(addButton);

    introLayout->addWidget(eyebrow);
    introLayout->addWidget(titleLabel);
    introLayout->addWidget(subtitleLabel);
    introLayout->addSpacing(6);
    introLayout->addLayout(actionLayout);

    QHBoxLayout* contentLayout = new QHBoxLayout;
    contentLayout->setSpacing(18);

    QFrame* tableCard = UiTheme::createCard();
    QVBoxLayout* tableLayout = new QVBoxLayout(tableCard);
    tableLayout->setContentsMargins(20, 20, 20, 20);
    tableLayout->setSpacing(12);

    QLabel* tableTitle = new QLabel("课程清单");
    tableTitle->setProperty("role", "sectionTitle");

    QLabel* tableHint = new QLabel("这里展示全部课程信息。主操作放在页头，列表只保留查看和选择的职责。");
    tableHint->setProperty("role", "muted");
    tableHint->setWordWrap(true);

    table = new QTableWidget;
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({ "课程ID", "课程名称", "任课教师", "备注", "用户ID" });
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->setColumnHidden(0, true);
    table->setColumnHidden(4, true);
    UiTheme::polishTable(table);

    tableLayout->addWidget(tableTitle);
    tableLayout->addWidget(tableHint);
    tableLayout->addWidget(table, 1);

    QFrame* helperCard = UiTheme::createCard();
    helperCard->setFixedWidth(280);

    QVBoxLayout* helperLayout = new QVBoxLayout(helperCard);
    helperLayout->setContentsMargins(20, 20, 20, 20);
    helperLayout->setSpacing(14);

    QLabel* helperTitle = new QLabel("辅助说明");
    helperTitle->setProperty("role", "sectionTitle");

    QLabel* helperText = new QLabel("建议先把所有课程建立完整，再回到任务页为任务绑定课程。删除课程不会删除任务，但会失去课程关联。");
    helperText->setProperty("role", "muted");
    helperText->setWordWrap(true);

    QLabel* helperPointOne = new QLabel("1. 课程名称建议使用正式课名。");
    helperPointOne->setProperty("role", "muted");
    helperPointOne->setWordWrap(true);

    QLabel* helperPointTwo = new QLabel("2. 任课教师字段方便后续快速识别。");
    helperPointTwo->setProperty("role", "muted");
    helperPointTwo->setWordWrap(true);

    QLabel* helperPointThree = new QLabel("3. 备注可记录时间、地点或作业习惯。");
    helperPointThree->setProperty("role", "muted");
    helperPointThree->setWordWrap(true);

    helperLayout->addWidget(helperTitle);
    helperLayout->addWidget(helperText);
    helperLayout->addWidget(helperPointOne);
    helperLayout->addWidget(helperPointTwo);
    helperLayout->addWidget(helperPointThree);
    helperLayout->addStretch();

    contentLayout->addWidget(tableCard, 1);
    contentLayout->addWidget(helperCard);

    pageLayout->addWidget(introCard);
    pageLayout->addLayout(contentLayout, 1);

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

    summaryLabel->setText(QString::number(courses.size()));
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
    dialog.resize(480, 360);

    QVBoxLayout* rootLayout = new QVBoxLayout(&dialog);
    rootLayout->setContentsMargins(24, 24, 24, 24);
    rootLayout->setSpacing(16);

    QLabel* dialogTitle = new QLabel(title);
    dialogTitle->setProperty("role", "sectionTitle");

    QLabel* dialogSubtitle = new QLabel("课程信息越规范，后续任务归类和统计分析就越清楚。");
    dialogSubtitle->setProperty("role", "muted");
    dialogSubtitle->setWordWrap(true);

    QLineEdit* nameEdit = new QLineEdit(courseName);
    nameEdit->setPlaceholderText("请输入课程名称");

    QLineEdit* teacherEdit = new QLineEdit(teacherName);
    teacherEdit->setPlaceholderText("请输入任课教师");

    QTextEdit* noteEdit = new QTextEdit(note);
    noteEdit->setPlaceholderText("可填写上课信息、课程要求或其他备注");
    noteEdit->setFixedHeight(120);

    QFormLayout* formLayout = new QFormLayout;
    formLayout->setLabelAlignment(Qt::AlignLeft);
    formLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    formLayout->setHorizontalSpacing(16);
    formLayout->setVerticalSpacing(14);
    formLayout->addRow("课程名称", nameEdit);
    formLayout->addRow("任课教师", teacherEdit);
    formLayout->addRow("备注", noteEdit);

    QPushButton* cancelButton = new QPushButton("取消");
    QPushButton* okButton = new QPushButton("保存课程");
    UiTheme::setButtonVariant(okButton, "primary");

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(okButton);

    rootLayout->addWidget(dialogTitle);
    rootLayout->addWidget(dialogSubtitle);
    rootLayout->addLayout(formLayout);
    rootLayout->addStretch();
    rootLayout->addLayout(buttonLayout);

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
    if (!showCourseDialog("新增课程", courseName, teacherName, note)) {
        return;
    }

    QString errorMessage;
    if (!manager->addCourse(courseName, teacherName, note, errorMessage)) {
        QMessageBox::warning(this, "新增失败", errorMessage);
        return;
    }
    refresh();
    emit dataChanged();
}

void CoursePage::editCourse()
{
    const QString courseId = selectedCourseId();
    if (courseId.isEmpty()) {
        QMessageBox::information(this, "需要先选择课程", "请先选择一门课程再进行编辑。");
        return;
    }

    const int row = table->currentRow();
    QString courseName = table->item(row, 1)->text();
    QString teacherName = table->item(row, 2)->text();
    QString note = table->item(row, 3)->text();

    if (!showCourseDialog("编辑课程", courseName, teacherName, note)) {
        return;
    }

    QString errorMessage;
    if (!manager->updateCourse(courseId, courseName, teacherName, note, errorMessage)) {
        QMessageBox::warning(this, "编辑失败", errorMessage);
        return;
    }
    refresh();
    emit dataChanged();
}

void CoursePage::deleteCourse()
{
    const QString courseId = selectedCourseId();
    if (courseId.isEmpty()) {
        QMessageBox::information(this, "需要先选择课程", "请先选择一门课程再进行删除。");
        return;
    }

    if (QMessageBox::question(this,
                              "删除课程",
                              "确认删除这门课程吗？已关联的任务会保留，但会失去课程关联。")
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
