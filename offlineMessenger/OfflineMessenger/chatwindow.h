#pragma once
#include <QMainWindow>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = 0);
    void reloadWindow();
    ~ChatWindow();

private slots:
    void on_btnSend_clicked();

private:
    Ui::ChatWindow *ui;
};

