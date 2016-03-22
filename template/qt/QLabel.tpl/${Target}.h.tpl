#ifndef _${Target}_H_${timestamp}_
#define _${Target}_H_${timestamp}_

#include <QLabel>
#include <QMouseEvent>

class ${Target} : public QLabel
{
    Q_OBJECT;
public:
    explicit ${Target}(QLabel *parent = 0);

protected:
    void mouseMoveEvent(QMouseEvent *event);

signals:

public slots:
};

#endif // _${Target}_H_${timestamp}_
