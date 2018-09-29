#include "send.h"
#include "ui_send.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QTableView>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QTextStream>

Send::Send(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Send)
{
    ui->setupUi(this);
}

Send::~Send()
{
    delete ui;
}

//
int Send::exec(void){
    QString s,s1;


    QFile file1("./output.txt");
    QFile file("./numebr.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        s=line;
        QStringList fields = line.split(",");
       // model->appendRow(fields);
    }


    file.close();

    //QFile file("farey.txt");
        file1.open(QFile::ReadOnly | QFile::Text);
        QTextStream ReadFile(&file1);
        while (!ReadFile.atEnd()) {
            QString line = ReadFile.readLine();
            s1=line;
           // ui->Output->append(line);
        }
        file.close();
QString ss;
ss="количество продуктов:"+s + " " + " Адрес дропа" + s1;
ui->label->setText(QString(ss));

int r=0;
r=QDialog::exec();
return r;
}


void Send::on_pushButton_clicked()
{
    accept();
}
