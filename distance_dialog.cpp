#include "distance_dialog.h"
#include "ui_distance_dialog.h"

Distance_dialog::Distance_dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Distance_dialog)
{
    ui->setupUi(this);
}

Distance_dialog::~Distance_dialog()
{
    delete ui;
}
