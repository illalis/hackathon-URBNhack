/********************************************************************************
** Form generated from reading UI file 'product.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRODUCT_H
#define UI_PRODUCT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Product
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *Product)
    {
        if (Product->objectName().isEmpty())
            Product->setObjectName(QStringLiteral("Product"));
        Product->resize(327, 350);
        gridLayout = new QGridLayout(Product);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pushButton = new QPushButton(Product);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 0, 0, 1, 1);

        pushButton_2 = new QPushButton(Product);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 1, 0, 1, 1);


        retranslateUi(Product);

        QMetaObject::connectSlotsByName(Product);
    } // setupUi

    void retranslateUi(QDialog *Product)
    {
        Product->setWindowTitle(QApplication::translate("Product", "Dialog", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Product", "\320\277\320\276\320\277\320\276\320\273\320\275\320\265\320\275\320\270\320\265 \320\261\320\260\320\267\321\213 \320\277\321\200\320\276\320\264\321\203\320\272\321\202\320\276\320\262", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("Product", "\320\264\321\200\320\276\320\277", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Product: public Ui_Product {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRODUCT_H
