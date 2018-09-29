#include "product.h"
#include "ui_product.h"
#include "basedata.h"
Product::Product(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Product)
{
    ui->setupUi(this);
}

Product::~Product()
{
    delete ui;
}

void Product::on_pushButton_clicked()
{
    auto bd= new Basedata(this);
    bd->exec();
    accept();

}
