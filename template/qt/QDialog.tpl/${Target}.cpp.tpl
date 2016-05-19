#include "${Target}.h"

#include <QComboBox>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QDebug>

#ifndef VALUE_MSG
#define VALUE_MSG(a) (#a) << '=' << (a)
#endif

${Target}::${Target}(QWidget *parent) :
    QDialog(parent)
{
    createMenu();
    createHorizontalGroupBox();
    createGridGroupBox();
    createFromGroupBox();

    bigEditor = new QTextEdit(this);
    bigEditor->setPlainText(tr("This widget takes up all the remaining space "
                               "in the top-level layout."));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                     QDialogButtonBox::Cancel, this);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &${Target}::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &${Target}::reject);


    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->setMenuBar(menuBar);

    mainLayout->addWidget(horizontalGroupBox);
    mainLayout->addWidget(gridGroupBox);
    mainLayout->addWidget(fromGroupBox);
    mainLayout->addWidget(bigEditor);
    mainLayout->addWidget(buttonBox);

    this->setLayout(mainLayout);

    this->setWindowTitle(tr("Basic Layouts"));
}

void ${Target}::createMenu()
{
    this->menuBar = new QMenuBar(this);
    fileMenu = new QMenu(tr("&File"), this);
    exitAction = fileMenu->addAction(tr("E&xit"));
    menuBar->addMenu(fileMenu);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(accept()));
    // connect(exitAction, &QAction::triggered, &${Target}::accept);
}

void ${Target}::createHorizontalGroupBox()
{
    this->horizontalGroupBox = new QGroupBox(tr("Horizontal layout"), this);
    QHBoxLayout * layout = new QHBoxLayout;

    for (int i = 0; i < NumButtons; ++i) {
        buttons[i] = new QPushButton(tr("Button %1").arg(i + 1), this);
        layout->addWidget(buttons[i]);
    }
    horizontalGroupBox->setLayout(layout);
}

void ${Target}::createGridGroupBox()
{
    this->gridGroupBox = new QGroupBox(tr("Grid layout"));

    auto * layout = new QGridLayout;
    for (int i = 0; i < std::distance(std::begin(lables), std::end(lables)); ++i) {
        lables[i] = new QLabel(tr("Line %1:").arg(i + 1), this);
        lineEdits[i] = new QLineEdit(this);
        layout->addWidget(lables[i],    i + 1, 0);
        layout->addWidget(lineEdits[i], i + 1, 1);
    }
    smallEditor = new QTextEdit(this);
    smallEditor->setPlainText(tr("This widget takes up about two thirds of the "
                                 "grid layout."));
    layout->addWidget(smallEditor, 0, 2, 4, 1);
    // 拉伸因子：让0列，最小(默认0)；第二列为10；第三列为20；
    // 也就是说，如果窗体被往右侧拉伸，那么每拉伸30个像素，第1、2列分享10、20像素；第0列，不变；
    // layout->setColumnStretch(0,  5);
    layout->setColumnStretch(1, 10);
    layout->setColumnStretch(2, 20);
    gridGroupBox->setLayout(layout);
}

void ${Target}::createFromGroupBox()
{
    fromGroupBox = new QGroupBox(tr("Form layout"));
    auto * layout = new QFormLayout;
    layout->addRow(new QLabel(tr("Line 1:"), this),             new QLineEdit(this));
    layout->addRow(new QLabel(tr("Line 2, long text:"), this),  new QComboBox(this));
    layout->addRow(new QLabel(tr("Line 3:"), this),             new QSpinBox(this));
    fromGroupBox->setLayout(layout);
}

