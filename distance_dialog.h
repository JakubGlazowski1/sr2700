#ifndef DISTANCE_DIALOG_H
#define DISTANCE_DIALOG_H

#include <QDialog>

namespace Ui {
class Distance_dialog;
}

class Distance_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Distance_dialog(QWidget *parent = nullptr);
    ~Distance_dialog();

private:
    Ui::Distance_dialog *ui;
};

#endif // DISTANCE_DIALOG_H
