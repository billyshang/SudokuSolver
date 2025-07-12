#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QEvent>
#include <QPaintEvent>
#include <QPushButton>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onSolveClicked();
    void onClearClicked();

private:
    Ui::MainWindow *ui;
    QLineEdit* grid[9][9];
    QPushButton* solveButton;
    QPushButton* clearButton;
    QLabel* titleLabel;
    QLabel* backgroundLabel;
    int cellSize;
    int cellSpacing;

    void setupUI();
    void setupGrid();
    void setupButtons();
    void setupTitle();
    void setupBackground();
    bool isValidInput(const QString& input);
    void resizeGrid();
    void resizeButtons();
    void resizeTitle();
};
#endif // MAINWINDOW_H
