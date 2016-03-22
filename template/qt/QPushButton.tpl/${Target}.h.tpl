#ifndef _${Target}_H_${timestamp}_
#define _${Target}_H_${timestamp}_

#include <QPushButton>

class ${Target} : public QPushButton
{
    Q_OBJECT;
public:
    explicit ${Target}(QPushButton *parent = 0);

signals:

public slots:

private:
    void onButtonCliecked();
};

#endif // _${Target}_H_${timestamp}_
