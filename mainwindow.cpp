#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "input_dialog.h"
#include "camera_dialog.h"
#include "distance_dialog.h"
#include "parameters_dialog.h"
#include "program_dialog.h"
#include "camera_widget.h"

#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    infoLabel = new QLabel(this);
    ui->setupUi(this);

    //setting up stacked widget
    ui->general_buttons_stackedWidget->raise();
    //set the position of the stacked widget
    ui->general_buttons_stackedWidget->move(5,220);
    ui->centralwidget->backgroundRole();
    ui->general_buttons_stackedWidgetPage1->setStyleSheet("background: transparent;");
    ui->page_3->setStyleSheet("background: transparent;");
    ui->page->setStyleSheet("background: transparent;");
    ui->page_2->setStyleSheet("background: transparent;");



    //setting main page from stacked widget
    ui->general_buttons_stackedWidget->setCurrentWidget(ui->general_buttons_stackedWidgetPage1);


}

void MainWindow::displayScreenInfo() {
    // Get the primary screen
    QScreen *screen = QApplication::primaryScreen();
    if (!screen) {
        infoLabel->setText("Error: No screen detected!");
        return;
    }

    // Get pixel resolution
    QSize pixelSize = screen->size();
    int width = pixelSize.width();
    int height = pixelSize.height();

    // Calculate aspect ratio
    int gcd = std::gcd(width, height);
    QString aspectRatio = QString("%1:%2").arg(width / gcd).arg(height / gcd);

    // Display the information
    QString screenInfo = QString(
                             "Screen Resolution: %1x%2 pixels\n"
                             "Aspect Ratio: %3"
                             )
                             .arg(width).arg(height)
                             .arg(aspectRatio);

    infoLabel->setText(screenInfo);

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_printingButton_clicked()
{
//wraca sie do pozycji domowej
}


void MainWindow::on_programsButton_clicked()
{
    program_dialog *dialog = new program_dialog();
    dialog ->show();

}


//void MainWindow::on_pushButton_2_clicked()
//{}


void MainWindow::on_lock_button_clicked()
{

    //graphic changes on clicking
    ui->lock_button->setStyleSheet("QPushButton { border: none; padding: 5px; } "
                                   "QPushButton:hover { background-color: transparent; }");
    ui->lock_button->setIconSize(QSize(41,41));
    if(isLocked){
        this->isLocked = false;
        ui->lock_button->setIcon(QIcon(":/images/images/unlock.png"));
    }
    else{
        this->isLocked = true;
        ui->lock_button->setIcon(QIcon(":/images/images/lock.png"));
    }

}


void MainWindow::on_start_button_clicked()
{

    //graphic changes on clicking on the button
    ui->start_button->setStyleSheet("QPushButton { border: none; padding: 5px; } "
                                   "QPushButton:hover { background-color: transparent; }");
    ui->start_button->setIconSize(QSize(42,42));
    if(isRunning){
        this->isRunning = false;
        ui->start_button->setIcon(QIcon(":/images/images/on_btn.png"));
    }
    else{
        this->isRunning = true;
        ui->start_button->setIcon(QIcon(":/images/images/off_btn.png"));
    }

}


void MainWindow::on_alignmentButton_clicked()
{
    //Camera_dialog *dialog = new Camera_dialog(this, ui->general_buttons_stackedWidgetPage1);
     //   dialog -> show();


    Camera_widget *cameraWidget= new Camera_widget(this);
    //cameraWidget->set

    ui->general_buttons_stackedWidget->insertWidget(1,cameraWidget);
    //cameraWidget->adjustSize();
    ui->general_buttons_stackedWidget->setCurrentIndex(1);
   // ui->camera_page->setStyleSheet("background: gray;");
}

