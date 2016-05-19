#ifndef _${Target}_H_${timestamp}_
#define _${Target}_H_${timestamp}_

#include <QDialog>

#include <QMenuBar>
#include <QGroupBox>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDialogButtonBox>

#include <QMenu>
#include <QAction>

class ${Target} : public QDialog
{
    Q_OBJECT;
public:
    // ${Target}();
    explicit ${Target}(QWidget *parent = 0);

private:
    void createMenu();
    void createHorizontalGroupBox();
    void createGridGroupBox();
    void createFromGroupBox();

private:
    // enum { NumGridRows = 3, NumButtons = 4};
    static const int NumGridRows = 3;
    static const int NumButtons = 4;

    QMenuBar * menuBar;
    QGroupBox * horizontalGroupBox;
    QGroupBox * gridGroupBox;
    QGroupBox * fromGroupBox;
    QTextEdit * smallEditor;
    QTextEdit * bigEditor;
    QLabel * lables[NumGridRows];
    QLineEdit *lineEdits[NumGridRows];
    QPushButton * buttons[NumButtons];
    QDialogButtonBox * buttonBox;

    QMenu * fileMenu;
    QAction * exitAction;
};

#endif // _${Target}_H_${timestamp}_

