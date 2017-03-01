#include "mainwindow.h"

#include <QtWidgets>
#include <QIcon>
#include <QKeySequence>
#include <QMenu>
#include <QToolBar>
#include <QAction>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setWindowTitle(tr("Main Window"));

    actionOpen = new QAction(QIcon::fromTheme("document-open"), tr("&Open..."), this);
    actionOpen->setShortcuts(QKeySequence::Open);
    actionOpen->setStatusTip(tr("Open an existing file."));
    connect(actionOpen, &QAction::triggered, this, &MainWindow::on_actionOpen_triggered);

    actionQuit = new QAction(QIcon::fromTheme("application-exit"), tr("&Quit"), this);
    actionQuit->setShortcut(QKeySequence::Quit);
    actionQuit->setToolTip(tr("Quit Application"));
    connect(actionQuit, &QAction::triggered, this, &MainWindow::on_actionQuit_triggered);

    QMenu * file =  this->menuBar()->addMenu(tr("&File"));
    file->addAction(actionOpen);
    file->addAction(actionQuit);

    QToolBar *toolBar = this->addToolBar(tr("&File"));
    toolBar->addAction(actionOpen);
    toolBar->addAction(actionQuit);

    (void)this->statusBar();
}

void MainWindow::on_actionOpen_triggered()
{
    // TODO
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

