#include "camera_dialog.h"
#include "ui_camera_dialog.h"
#include <QVideoWidget>
#include <QMediaDevices>


Camera_dialog::Camera_dialog(QWidget *parent, QWidget *targetWidget)
    : QDialog(parent)
    , ui(new Ui::Camera_dialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    if (targetWidget && parentWidget()) {
        // Get the global position of targetWidget
        //QPoint targetPos = targetWidget->pos();
        // Convert to local coordinates of the dialog's parent (main window)
        QPoint dialogPos = targetWidget->pos();
        // Move dialog to cover targetWidget
        move(dialogPos);
    }

    // Create video widget to display camera feed
    videoWidget1 = new QVideoWidget(ui->widget);
    videoWidget2 = new QVideoWidget(ui->widget_2);

    //Resize it to the ui oryginal widgets
    QSize videoVidget1Size = ui->widget->size();
    QSize videoVidget2Size = ui->widget_2->size();
    videoWidget2->resize(videoVidget2Size);
    videoWidget1->resize(videoVidget1Size);

    // Create media capture session

    // Set up the camera
    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    if (!cameras.isEmpty() && cameras.size()>1) {   //check if there are 2 cameras available
        captureSession1 = new QMediaCaptureSession(this);
        camera1 = new QCamera(cameras.first(), this); // Use the first available camera
        captureSession1->setCamera(camera1);
        captureSession1->setVideoOutput(videoWidget1);
        camera1->start(); // Start the camera automatically

        captureSession2 = new QMediaCaptureSession(this);
        camera2 = new QCamera(cameras.at(1), this); // Use the second available camera
        captureSession2->setCamera(camera2);
        captureSession2->setVideoOutput(videoWidget2);
        camera2->start(); // Start the camera automatically
    }
    else {
        handleCameraError(QCamera::CameraError); //If error ocured, handle error
    }

}

void Camera_dialog::handleCameraError(QCamera::Error error)
{
    qDebug() << "Camera error:" << error;
    ui->label->setText("Camera error occurred");
}
Camera_dialog::~Camera_dialog()
{
    camera1->stop();
    camera2->stop();
    delete ui;
}

void Camera_dialog::on_pushButton_clicked()
{
    this->close();
}

