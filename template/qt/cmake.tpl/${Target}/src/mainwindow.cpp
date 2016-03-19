#include "mainwindow.h"

#include <QtWidgets>
#include <QIcon>
#include <QKeySequence>
#include <QMenu>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setWindowTitle(tr("Main Window"));

    openAction = new QAction(QIcon(":/images/doc-open"),
                             tr("&Open..."),
                             this);

    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file."));
    connect(openAction, &QAction::triggered, this, &MainWindow::open);

    QMenu * file =  this->menuBar()->addMenu(tr("&File"));
    file->addAction(openAction);

    QToolBar *toolBar = this->addToolBar(tr("&File"));
    toolBar->addAction(openAction);
}

void MainWindow::open()
{
    // TODO
}

