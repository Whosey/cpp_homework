#include "UiTheme.h"

#include <QApplication>
#include <QFrame>
#include <QHeaderView>
#include <QPushButton>
#include <QTableWidget>

namespace UiTheme {

namespace {
void repolish(QWidget* widget)
{
    if (!widget) {
        return;
    }
    widget->style()->unpolish(widget);
    widget->style()->polish(widget);
    widget->update();
}
}

void applyApplicationStyle(QApplication& app)
{
    app.setStyleSheet(R"(
        QWidget {
            background: #0F172A;
            color: #E2E8F0;
            font-family: "Microsoft YaHei", "Microsoft YaHei UI", "PingFang SC", "Segoe UI", sans-serif;
            font-size: 14px;
        }
        QMainWindow, QDialog {
            background: #0F172A;
        }
        QLabel {
            background: transparent;
        }
        QFrame#card {
            background: #1E293B;
            border: 1px solid #334155;
            border-radius: 22px;
        }
        QFrame#sidebar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #172554, stop:1 #0F172A);
            border: 1px solid #334155;
            border-radius: 28px;
        }
        QLabel[role="eyebrow"] {
            color: #94A3B8;
            font-size: 12px;
            font-weight: 600;
        }
        QLabel[role="hero"] {
            font-size: 24px;
            font-weight: 600;
            color: #E2E8F0;
        }
        QLabel[role="pageTitle"] {
            font-size: 22px;
            font-weight: 600;
            color: #E2E8F0;
        }
        QLabel[role="sectionTitle"] {
            font-size: 18px;
            font-weight: 600;
            color: #E2E8F0;
        }
        QLabel[role="cardTitle"] {
            font-size: 16px;
            font-weight: 600;
            color: #E2E8F0;
        }
        QLabel[role="muted"], QLabel[role="sidebarMuted"] {
            color: #94A3B8;
        }
        QLabel[role="sidebarMuted"] {
            color: #CBD5E1;
        }
        QLabel[role="sidebarTitle"] {
            color: #E2E8F0;
            font-size: 18px;
            font-weight: 600;
        }
        QLabel[role="metricValue"] {
            font-size: 28px;
            font-weight: 600;
            color: #E2E8F0;
        }
        QLabel[role="metricCaption"] {
            color: #94A3B8;
            font-size: 13px;
        }
        QLabel[role="highlight"] {
            color: #E2E8F0;
            font-size: 15px;
            font-weight: 600;
        }
        QLabel[role="hint"] {
            color: #CBD5E1;
            line-height: 1.4;
        }
        QLabel[role="accent"] {
            color: #3B82F6;
            font-size: 13px;
            font-weight: 700;
        }
        QLabel[role="success"] {
            color: #22C55E;
            font-weight: 700;
        }
        QLabel[role="warning"] {
            color: #F59E0B;
            font-weight: 700;
        }
        QLabel[role="danger"] {
            color: #EF4444;
            font-weight: 700;
        }
        QLineEdit, QTextEdit, QComboBox, QDateEdit {
            background: #0F172A;
            border: 1px solid #334155;
            border-radius: 12px;
            padding: 10px 12px;
            selection-background-color: #3B82F6;
            selection-color: #E2E8F0;
        }
        QLineEdit:focus, QTextEdit:focus, QComboBox:focus, QDateEdit:focus {
            border: 1px solid #3B82F6;
            background: #111C33;
        }
        QTextEdit {
            padding: 12px;
        }
        QComboBox::drop-down, QDateEdit::drop-down {
            border: 0;
            width: 24px;
        }
        QComboBox QAbstractItemView {
            background: #1E293B;
            color: #E2E8F0;
            border: 1px solid #334155;
            selection-background-color: #3B82F6;
            selection-color: #E2E8F0;
        }
        QPushButton {
            border-radius: 12px;
            border: 1px solid #334155;
            padding: 10px 16px;
            font-weight: 600;
            background: #1E293B;
            color: #E2E8F0;
        }
        QPushButton:hover {
            background: #23324A;
            border-color: #3B82F6;
        }
        QPushButton:pressed {
            background: #162033;
        }
        QPushButton[variant="primary"] {
            background: #3B82F6;
            color: #E2E8F0;
            border-color: #3B82F6;
        }
        QPushButton[variant="primary"]:hover {
            background: #2563EB;
            border-color: #2563EB;
        }
        QPushButton[variant="secondary"] {
            background: #1E293B;
            color: #E2E8F0;
            border-color: #3B82F6;
        }
        QPushButton[variant="secondary"]:hover {
            background: #23324A;
        }
        QPushButton[variant="danger"] {
            background: rgba(239, 68, 68, 0.12);
            color: #FCA5A5;
            border-color: rgba(239, 68, 68, 0.4);
        }
        QPushButton[variant="danger"]:hover {
            background: rgba(239, 68, 68, 0.18);
        }
        QPushButton[variant="ghost"] {
            background: transparent;
            border-color: transparent;
            color: #94A3B8;
        }
        QPushButton[variant="ghost"]:hover {
            background: rgba(59, 130, 246, 0.10);
            border-color: transparent;
        }
        QPushButton[variant="nav"] {
            text-align: left;
            padding: 14px 16px;
            color: #CBD5E1;
            background: transparent;
            border-color: transparent;
        }
        QPushButton[variant="nav"]:hover {
            background: rgba(59, 130, 246, 0.12);
            border-color: transparent;
        }
        QPushButton[variant="nav"]:checked {
            color: #E2E8F0;
            background: rgba(59, 130, 246, 0.18);
            border: 1px solid rgba(59, 130, 246, 0.45);
        }
        QTableWidget {
            background: #0F172A;
            alternate-background-color: #111C33;
            gridline-color: #334155;
            border: 0;
            border-radius: 14px;
            selection-background-color: rgba(59, 130, 246, 0.28);
            selection-color: #E2E8F0;
        }
        QTableWidget::item {
            padding: 10px;
            border-bottom: 1px solid #1E293B;
        }
        QHeaderView::section {
            background: #111C33;
            color: #94A3B8;
            border: 0;
            border-bottom: 1px solid #334155;
            padding: 12px 10px;
            font-weight: 700;
        }
        QListWidget {
            background: #0F172A;
            border: 0;
            border-radius: 14px;
            padding: 6px;
        }
        QListWidget::item {
            background: #111C33;
            border: 1px solid #334155;
            border-radius: 12px;
            margin: 6px 0;
            padding: 12px;
        }
        QProgressBar {
            background: #111C33;
            border: 1px solid #334155;
            border-radius: 8px;
            text-align: center;
            min-height: 14px;
            color: #E2E8F0;
        }
        QProgressBar::chunk {
            background: #3B82F6;
            border-radius: 8px;
        }
        QScrollBar:vertical {
            background: transparent;
            width: 12px;
            margin: 6px 0;
        }
        QScrollBar::handle:vertical {
            background: #334155;
            border-radius: 6px;
            min-height: 28px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical,
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical,
        QScrollBar:horizontal, QScrollBar::handle:horizontal,
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal,
        QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
            background: transparent;
            border: 0;
            height: 0;
            width: 0;
        }
        QGroupBox {
            font-weight: 700;
            border: 1px solid #334155;
            border-radius: 16px;
            margin-top: 10px;
            padding-top: 18px;
            background: #1E293B;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 16px;
            padding: 0 4px;
            color: #E2E8F0;
        }
        QCheckBox {
            color: #E2E8F0;
            spacing: 8px;
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border-radius: 5px;
            border: 1px solid #334155;
            background: #0F172A;
        }
        QCheckBox::indicator:checked {
            background: #3B82F6;
            border-color: #3B82F6;
        }
        QMessageBox QLabel {
            min-width: 320px;
        }
    )");
}

void setButtonVariant(QPushButton* button, const QString& variant)
{
    if (!button) {
        return;
    }
    button->setProperty("variant", variant);
    repolish(button);
}

void polishTable(QTableWidget* table)
{
    if (!table) {
        return;
    }

    table->setAlternatingRowColors(true);
    table->setShowGrid(false);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setHighlightSections(false);
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setFocusPolicy(Qt::NoFocus);
    table->setStyleSheet("QTableCornerButton::section{background:#111C33;border:0;border-bottom:1px solid #334155;}");
}

QFrame* createCard(QWidget* parent)
{
    QFrame* frame = new QFrame(parent);
    frame->setObjectName("card");
    return frame;
}

}
