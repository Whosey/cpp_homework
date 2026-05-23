#ifndef UITHEME_H
#define UITHEME_H

#include <QString>

class QApplication;
class QFrame;
class QPushButton;
class QTableWidget;
class QWidget;

namespace UiTheme {

void applyApplicationStyle(QApplication& app);
void setButtonVariant(QPushButton* button, const QString& variant);
void polishTable(QTableWidget* table);
QFrame* createCard(QWidget* parent = nullptr);

}

#endif
