#ifndef DROP_H
#define DROP_H

#include <QDialog>

namespace Ui {
class Drop;
}

class Drop : public QDialog
{
    Q_OBJECT

public:
    explicit Drop(QWidget *parent = 0);
    ~Drop();
     int exec(void);

private slots:
    void setPlaceHolderText(const QString &);




    void on_pushButton_clicked();

private:
    Ui::Drop *ui;
};

#endif // DROP_H
