#ifndef SEND_H
#define SEND_H

#include <QDialog>

namespace Ui {
class Send;
}

class Send : public QDialog
{
    Q_OBJECT

public:
    explicit Send(QWidget *parent = 0);
    ~Send();
    int exec(void);



private slots:
    void on_pushButton_clicked();

private:
    Ui::Send *ui;
};

#endif // SEND_H
