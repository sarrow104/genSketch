#ifndef _${Target}_H_${timestamp}_
#define _${Target}_H_${timestamp}_

#include <QDialog>

class QPaintEvent;

class ${Target} : public QDialog
{
    Q_OBJECT;
public:
    explicit ${Target}(QWidget *parent = 0);

protected:

    void paintEvent(QPaintEvent* e);

private:

};

#endif // _${Target}_H_${timestamp}_

