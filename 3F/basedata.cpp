#include "basedata.h"
#include "ui_basedata.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QTableView>
#include <QMessageBox>

Basedata::Basedata(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Basedata)
{
    ui->setupUi(this);
}

Basedata::~Basedata()
{
    delete ui;
}

int Basedata::exec(void){

    int r=0;
    setPlaceHolderText("123");
    r=QDialog::exec();
    return r;

}

void Basedata::setPlaceHolderText(const QString &){

    ui->naimenovanie->setPlaceholderText("Наименование продукта (Молоко, хлеб и т.д.)");
    ui->count->setPlaceholderText("Количество");
}

