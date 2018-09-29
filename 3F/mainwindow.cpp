#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "product.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QTableView>
#include <QUrl>
#include <QMessageBox>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{   ui->label_2->setStyleSheet("background-image: url(./ashan960.jpg)");

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;

}


void MainWindow::on_pushButton_clicked()
{
    auto p = new Product(this);
    p->exec();

}

void MainWindow::on_pushButton_2_clicked()
{
  close();
}
