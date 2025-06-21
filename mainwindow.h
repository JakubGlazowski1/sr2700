#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_printingButton_clicked();

    void on_programsButton_clicked();

   // void on_pushButton_2_clicked();

    void on_lock_button_clicked();

    void on_start_button_clicked();

    void on_alignmentButton_clicked();

private:
    Ui::MainWindow *ui;
    void displayScreenInfo();
    QLabel *infoLabel;
    bool isLocked = false;
    bool isRunning = false;
};
#endif // MAINWINDOW_H
