#include "program_dialog.h"
#include "ui_program_dialog.h"

program_dialog::program_dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::program_dialog)
{
    ui->setupUi(this);
}

program_dialog::~program_dialog()
{
    delete ui;
}
