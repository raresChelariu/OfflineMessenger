#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once
#include <QMainWindow>
#include "appmess.h"
#include "chatwindow.h"
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnLogin_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
