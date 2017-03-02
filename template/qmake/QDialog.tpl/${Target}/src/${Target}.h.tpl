#pragma once

#include <QDialog>

namespace Ui {
class ${Target};
}

class ${Target} : public QDialog
{
    Q_OBJECT

public:
    explicit ${Target}(QWidget *parent = 0);
    ~${Target}();

private slots:
    void on_pushButton_clicked();

private:
    Ui::${Target} *ui;
};

