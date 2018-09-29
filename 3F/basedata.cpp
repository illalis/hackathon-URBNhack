#include "basedata.h"
#include "ui_basedata.h"
#include "models/storagemodel.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QTableView>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

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


void Basedata::on_pushButton_clicked()
{
    auto base = new StorageModel("./products.csv");

    base->setObjectName("products");
    base->setTitle("new_added_products");
    QStringList headers = {"Наименование","Количество", "Номер"};
    base->setHeaderData(headers);

    QString count_1;
    count_1= ui->count->text();
    auto row = base->rowCount()-1;
    if (count_1.toInt() > 0){
        for (int i=0; i<count_1.toInt();++i){
            base->insertRow(base->rowCount());
            base->setData(row+i,0, ui->naimenovanie->text());
            base->setData(row+i,1,"1");
            base->setData(row+i,2, QString::number(base->rowCount()));


        }
    }
    base->saveToDisk();
   QString ss= QString::number(base->rowCount());
   QFile file("numebr.txt");

      if(file.open(QFile::WriteOnly | QFile::Text)) {
          QTextStream stream(&file);
          stream << ss.toInt() - 1;
          file.close();

    QDialog dialog(this);
    dialog.setObjectName("StorageDemoDialog");
    dialog.setWindowTitle(tr("продукты"));
    dialog.setSizeGripEnabled(true);

    auto priceView = new QTableView(&dialog);
    priceView->setModel(base);

    auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok, &dialog);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    auto layout = new QVBoxLayout(&dialog);
    layout->addWidget(priceView);
    layout->addWidget(buttons);
    dialog.setLayout(layout);
    dialog.resize(400, 200);
    dialog.exec();
}
}

void Basedata::on_pushButton_2_clicked()
{

 this->close();
}

void Basedata::on_pushButton_3_clicked()
{
    remove("./products.csv");
    remove("./numebr.txt");
}

