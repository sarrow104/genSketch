#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT;
public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow() = default;

signals:

public slots:
    void on_actionOpen_triggered();
    void on_actionQuit_triggered();

private:
    QAction * actionOpen;
    QAction * actionQuit;
};

#endif // MAINWINDOW_H
