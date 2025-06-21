#ifndef PARAMETERS_DIALOG_H
#define PARAMETERS_DIALOG_H

#include <QDialog>

namespace Ui {
class Parameters_dialog;
}

class Parameters_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Parameters_dialog(QWidget *parent = nullptr);
    ~Parameters_dialog();

private:
    Ui::Parameters_dialog *ui;
};

#endif // PARAMETERS_DIALOG_H
