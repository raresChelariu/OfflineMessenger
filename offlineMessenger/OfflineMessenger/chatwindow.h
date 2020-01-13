#pragma once
#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString receiver;
    explicit ChatWindow(QWidget *parent = 0);

    void getAllUsersList();
    void getOnlineUsersList();
    void reloadUsersList();
    int UserExistsAt(QString s);
    ~ChatWindow();
private slots:
    void on_btnSend_clicked();

    void reloadChat();
    void on_listUsers_itemClicked(QListWidgetItem *item);

    void on_btnRefreshUsersList_clicked();

private:
    Ui::ChatWindow *ui;
};

