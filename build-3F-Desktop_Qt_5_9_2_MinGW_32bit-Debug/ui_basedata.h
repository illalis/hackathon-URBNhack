/********************************************************************************
** Form generated from reading UI file 'basedata.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BASEDATA_H
#define UI_BASEDATA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Basedata
{
public:
    QGridLayout *gridLayout;
    QLineEdit *naimenovanie;
    QLineEdit *count;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *Basedata)
    {
        if (Basedata->objectName().isEmpty())
            Basedata->setObjectName(QStringLiteral("Basedata"));
        Basedata->resize(400, 300);
        gridLayout = new QGridLayout(Basedata);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        naimenovanie = new QLineEdit(Basedata);
        naimenovanie->setObjectName(QStringLiteral("naimenovanie"));

        gridLayout->addWidget(naimenovanie, 0, 0, 1, 1);

        count = new QLineEdit(Basedata);
        count->setObjectName(QStringLiteral("count"));

        gridLayout->addWidget(count, 1, 0, 1, 1);

        pushButton = new QPushButton(Basedata);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 2, 0, 1, 1);

        pushButton_2 = new QPushButton(Basedata);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 3, 0, 1, 1);


        retranslateUi(Basedata);

        QMetaObject::connectSlotsByName(Basedata);
    } // setupUi

    void retranslateUi(QDialog *Basedata)
    {
        Basedata->setWindowTitle(QApplication::translate("Basedata", "Dialog", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Basedata", "\320\223\320\276\321\202\320\276\320\262\320\276", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("Basedata", "\320\275\320\260\320\267\320\260\320\264", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Basedata: public Ui_Basedata {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BASEDATA_H
