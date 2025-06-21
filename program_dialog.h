#ifndef PROGRAM_DIALOG_H
#define PROGRAM_DIALOG_H

#include <QDialog>

namespace Ui {
class program_dialog;
}

class program_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit program_dialog(QWidget *parent = nullptr);
    ~program_dialog();

private:
    Ui::program_dialog *ui;
};

#endif // PROGRAM_DIALOG_H
