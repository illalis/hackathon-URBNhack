#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "product.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
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
