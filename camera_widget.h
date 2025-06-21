#ifndef CAMERA_WIDGET_H
#define CAMERA_WIDGET_H

#include <QWidget>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QCameraDevice>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaCaptureSession>

namespace Ui {
class Camera_widget;
}

class Camera_widget : public QWidget
{
    Q_OBJECT

public:
    explicit Camera_widget(QWidget *parent = nullptr);
    ~Camera_widget();

private:
    QVideoWidget *videoWidget1;
    QVideoWidget *videoWidget2;
    QCamera *camera1;
    QCamera *camera2;
    QMediaCaptureSession *captureSession1;
    QMediaCaptureSession *captureSession2;
    Ui::Camera_widget *ui;
private slots:
        void handleCameraError(QCamera::Error error);
    void on_pushButton_clicked();
};



#endif // CAMERA_WIDGET_H
