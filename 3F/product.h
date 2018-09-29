#ifndef PRODUCT_H
#define PRODUCT_H

#include <QDialog>

namespace Ui {
class Product;
}

class Product : public QDialog
{
    Q_OBJECT

public:
    explicit Product(QWidget *parent = 0);
    ~Product();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Product *ui;
};

#endif // PRODUCT_H
