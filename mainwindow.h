#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebView>
#include <QUrl>
#include "tilestream.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();   

private:
    Ui::MainWindow *ui;
    QWebView *view;
    TileStream server;
};

#endif // MAINWINDOW_H
