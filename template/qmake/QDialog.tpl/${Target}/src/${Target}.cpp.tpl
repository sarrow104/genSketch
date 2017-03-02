#include "${Target}.h"
#include "ui_${Target}.h"

${Target}::${Target}(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::${Target})
{
    ui->setupUi(this);
    // ui->listWidget->addItem("Hello");
    for (int i = 0; i < 9; ++i)
    {
        ui->listWidget->addItem(QString::number(i) + "item here");
    }
}

${Target}::~${Target}()
{
    delete ui;
}

void ${Target}::on_pushButton_clicked()
{
    // ui->listWidget->currentItem();
    auto item = ui->listWidget->currentItem();
    item->setTextColor(Qt::red);
    item->setBackgroundColor(Qt::gray);
    item->setText("Fuzzy Bunny");
}
