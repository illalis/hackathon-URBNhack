#include "drop.h"
#include "ui_drop.h"
#include "models/storagemodel.h"
#include "send.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QTableView>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

Drop::Drop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Drop)
{
    ui->setupUi(this);
}

Drop::~Drop()
{
    delete ui;
}

int Drop::exec(void){
    int r=0;
    setPlaceHolderText ("123");
    r=QDialog::exec();
    return r;


}

void Drop::setPlaceHolderText(const QString &){
        ui->droplace->setPlaceholderText("Адрес дропа");
}


void Drop::on_pushButton_clicked()
{

    QFile file("output.txt");
    QString ss;

       if(file.open(QFile::WriteOnly | QFile::Text)) {
           QTextStream stream(&file);
           stream << ui->droplace->text();
           file.close();

    auto sn = new Send(this);
    sn->exec();

accept();


}
}
