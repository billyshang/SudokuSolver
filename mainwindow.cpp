#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFile>
#include <QTextStream>
#include <QDebug>
// 假设 SudokuSolver 类定义在 sudokusolver.h 中
#include "sudokusolver.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    setupUI();
    this->setWindowIcon(QIcon(":/pic.ico"));
    this->setStyleSheet("background-color: #f0f5f9;"); // 设置窗口背景颜色

    // 为所有 QLineEdit 控件安装事件过滤器
    for(int i = 0; i < 9; ++i) {
        for(int j = 0; j < 9; ++j) {
            grid[i][j]->installEventFilter(this);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    for(int i = 0; i < 9; ++i) {
        for(int j = 0; j < 9; ++j) {
            delete grid[i][j];
        }
    }
    delete solveButton;
    delete clearButton;
    delete titleLabel;
    delete backgroundLabel;
}

void MainWindow::setupUI()
{
    setupBackground();
    setupGrid();
    setupButtons();
    setupTitle();
}

void MainWindow::setupGrid()
{
    for(int i = 0; i < 9; ++i) {
        for(int j = 0; j < 9; ++j) {
            grid[i][j] = new QLineEdit(this);
            grid[i][j]->setAlignment(Qt::AlignCenter);
            grid[i][j]->setMaxLength(1);
            grid[i][j]->setStyleSheet("background-color: #ffffff; border: 1px solid #ccc; border-radius: 5px; font-size: 20px; color: #333;");
        }
    }
    resizeGrid();
}

void MainWindow::setupButtons()
{
    solveButton = new QPushButton("求解", this);
    solveButton->setStyleSheet("QPushButton {"
                               "background-color: #4CAF50; "
                               "color: white; "
                               "border: none; "
                               "border-radius: 5px; "
                               "font-size: 16px; "
                               "box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.3); "
                               "}"
                               "QPushButton:hover {"
                               "background-color: #45a049; "
                               "}"
                               "QPushButton:pressed {"
                               "background-color: #3e8e41; "
                               "box-shadow: 1px 1px 3px rgba(0, 0, 0, 0.3); "
                               "}");
    connect(solveButton, &QPushButton::clicked, this, &MainWindow::onSolveClicked);

    clearButton = new QPushButton("清空", this);
    clearButton->setStyleSheet("QPushButton {"
                               "background-color: #f44336; "
                               "color: white; "
                               "border: none; "
                               "border-radius: 5px; "
                               "font-size: 16px; "
                               "box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.3); "
                               "}"
                               "QPushButton:hover {"
                               "background-color: #da190b; "
                               "}"
                               "QPushButton:pressed {"
                               "background-color: #c62828; "
                               "box-shadow: 1px 1px 3px rgba(0, 0, 0, 0.3); "
                               "}");
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    resizeButtons();
}

void MainWindow::setupTitle()
{
    titleLabel = new QLabel("数独求解器", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333;");
    resizeTitle();
}

void MainWindow::setupBackground()
{
    backgroundLabel = new QLabel(this);
    backgroundLabel->setGeometry(0, 0, this->width(), this->height());
    QPixmap backgroundImage(":/background.jpg");
    backgroundLabel->setPixmap(backgroundImage.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    backgroundLabel->lower();
}

void MainWindow::onSolveClicked()
{
    std::string input;
    for(int i = 0; i < 9; ++i) {
        for(int j = 0; j < 9; ++j) {
            QString text = grid[i][j]->text();
            if (!isValidInput(text)) {
                QMessageBox::warning(this, "错误", "输入无效，请输入 1 - 9 的数字或留空。");
                return;
            }
            input += text.isEmpty() ? '.' : text.at(0).toLatin1();
        }
    }

    SudokuSolver solver;
    char result[9][9];
    try {
        if(solver.solve(input, result)) {
            for(int i = 0; i < 9; ++i) {
                for(int j = 0; j < 9; ++j) {
                    grid[i][j]->setText(QChar(result[i][j]));
                }
            }
        } else {
            QMessageBox::warning(this, "错误", "无解！请检查输入");
        }
    } catch (const std::invalid_argument& e) {
        QMessageBox::warning(this, "错误", QString("输入无效：") + e.what());
    }
}

void MainWindow::onClearClicked()
{
    for(int i = 0; i < 9; ++i) {
        for(int j = 0; j < 9; ++j) {
            grid[i][j]->clear();
        }
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        QLineEdit *lineEdit = qobject_cast<QLineEdit*>(obj);
        if (lineEdit) {
            QString text = keyEvent->text();
            if (!isValidInput(text) && text != "") {
                return true; // 过滤无效输入
            }

            // 处理方向键事件
            int currentRow = -1, currentCol = -1;
            for(int i = 0; i < 9; ++i) {
                for(int j = 0; j < 9; ++j) {
                    if (grid[i][j] == lineEdit) {
                        currentRow = i;
                        currentCol = j;
                        break;
                    }
                }
                if (currentRow != -1) break;
            }

            if (currentRow != -1 && currentCol != -1) {
                switch (keyEvent->key()) {
                case Qt::Key_Up:
                    if (currentRow > 0) {
                        grid[currentRow - 1][currentCol]->setFocus();
                    }
                    return true;
                case Qt::Key_Down:
                    if (currentRow < 8) {
                        grid[currentRow + 1][currentCol]->setFocus();
                    }
                    return true;
                case Qt::Key_Left:
                    if (currentCol > 0) {
                        grid[currentRow][currentCol - 1]->setFocus();
                    }
                    return true;
                case Qt::Key_Right:
                    if (currentCol < 8) {
                        grid[currentRow][currentCol + 1]->setFocus();
                    }
                    return true;
                case Qt::Key_Backspace:
                case Qt::Key_Delete:
                    lineEdit->clear();
                    return true;
                case Qt::Key_Return: // 处理回车键
                case Qt::Key_Enter:
                    // 可以在这里添加按下回车键后的逻辑，比如直接求解
                    onSolveClicked();
                    return true;
                }
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    QPainter painter(this);
    QPen pen(Qt::gray, 2); // 大网格线颜色和宽度
    painter.setPen(pen);

    int gridLeft = (this->width() - 9 * cellSize - 8 * cellSpacing) / 2;
    int gridTop = (this->height() - 9 * cellSize - 8 * cellSpacing) / 2;

    // 绘制大网格线
    for (int i = 0; i <= 9; i += 3) {
        painter.drawLine(gridLeft, gridTop + i * (cellSize + cellSpacing), gridLeft + 9 * (cellSize + cellSpacing), gridTop + i * (cellSize + cellSpacing));
        painter.drawLine(gridLeft + i * (cellSize + cellSpacing), gridTop, gridLeft + i * (cellSize + cellSpacing), gridTop + 9 * (cellSize + cellSpacing));
    }

    // 绘制小网格线
    pen.setWidth(1);
    pen.setColor(Qt::lightGray); // 小网格线颜色
    painter.setPen(pen);
    for (int i = 0; i <= 9; ++i) {
        if (i % 3 != 0) {
            painter.drawLine(gridLeft, gridTop + i * (cellSize + cellSpacing), gridLeft + 9 * (cellSize + cellSpacing), gridTop + i * (cellSize + cellSpacing));
            painter.drawLine(gridLeft + i * (cellSize + cellSpacing), gridTop, gridLeft + i * (cellSize + cellSpacing), gridTop + 9 * (cellSize + cellSpacing));
        }
    }
}

bool MainWindow::isValidInput(const QString& input)
{
    if (input.isEmpty()) return true;
    bool ok;
    int num = input.toInt(&ok);
    return ok && num >= 1 && num <= 9;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    backgroundLabel->setGeometry(0, 0, this->width(), this->height());
    resizeGrid();
    resizeButtons();
    resizeTitle();
}

void MainWindow::resizeGrid()
{
    int availableWidth = this->width() - 100;
    int availableHeight = this->height() - 200;
    int minSize = qMin(availableWidth / 9, availableHeight / 9);
    cellSize = minSize;
    cellSpacing = 2;

    int gridLeft = (this->width() - 9 * cellSize - 8 * cellSpacing) / 2;
    int gridTop = (this->height() - 9 * cellSize - 8 * cellSpacing) / 2;

    for(int i = 0; i < 9; ++i) {
        for(int j = 0; j < 9; ++j) {
            grid[i][j]->setFixedSize(cellSize, cellSize);
            grid[i][j]->move(gridLeft + j * (cellSize + cellSpacing), gridTop + i * (cellSize + cellSpacing));
        }
    }
}

void MainWindow::resizeButtons()
{
    int buttonWidth = 80;
    int buttonHeight = 40;
    int solveButtonLeft = (this->width() - buttonWidth * 2 - 40) / 2;
    int clearButtonLeft = solveButtonLeft + buttonWidth + 40;
    int buttonTop = this->height() - buttonHeight - 50;

    solveButton->setGeometry(solveButtonLeft, buttonTop, buttonWidth, buttonHeight);
    clearButton->setGeometry(clearButtonLeft, buttonTop, buttonWidth, buttonHeight);
}

void MainWindow::resizeTitle()
{
    int titleWidth = 200;
    int titleHeight = 30;
    int titleLeft = (this->width() - titleWidth) / 2;
    int titleTop = 20;

    titleLabel->setGeometry(titleLeft, titleTop, titleWidth, titleHeight);
}
