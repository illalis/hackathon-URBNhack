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


