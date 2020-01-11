#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "appmess.h"

ChatWindow::ChatWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    char txt[256];
    sprintf(txt, "Logged as %s", AppMess::username);
    ui->labelUsername->setText(QString(AppMess::username));
    reloadWindow();
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::on_btnSend_clicked()
{
    char* msg = (char*) malloc(256);
    strcpy(msg, "sendmessage");
    write(AppMess::client->sockDescr, msg, 256);
    strcpy(msg, ui->leUserInput->text().toStdString().c_str());
    write(AppMess::client->sockDescr, msg, 256);
    free(msg);
}
void ChatWindow::reloadWindow()
{
    char* msg = (char*) malloc(256);
    int noMessages;
    write(AppMess::client->sockDescr, "getlist", 256);
    read(AppMess::client->sockDescr, &noMessages, sizeof(int));
    for (int i = 0; i < noMessages; i++)
    {
        read(AppMess::client->sockDescr, msg, 256);

    }
    free(msg);
}

