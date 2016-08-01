#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>

class MainWindow : public QMainWindow
{
    Q_OBJECT;
public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow() = default;

signals:

public slots:
    void open();

private:
    QAction * openAction;
};

#endif // MAINWINDOW_H
