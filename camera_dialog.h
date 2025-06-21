#ifndef CAMERA_DIALOG_H
#define CAMERA_DIALOG_H


#include <QDialog>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QCameraDevice>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaCaptureSession>

namespace Ui {
class Camera_dialog;
}

class Camera_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Camera_dialog(QWidget *parent = nullptr, QWidget *targetWidget = nullptr);
    ~Camera_dialog();

private:
    Ui::Camera_dialog *ui;
    QVideoWidget *videoWidget1;
    QVideoWidget *videoWidget2;
    QCamera *camera1;
    QCamera *camera2;
    QMediaCaptureSession *captureSession1;
    QMediaCaptureSession *captureSession2;
private slots:
    void handleCameraError(QCamera::Error error);
    void on_pushButton_clicked();
};

#endif // CAMERA_DIALOG_H
