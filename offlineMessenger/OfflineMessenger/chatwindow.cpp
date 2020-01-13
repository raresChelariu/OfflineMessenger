#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "appmess.h"
#include <QTimer>

ChatWindow::ChatWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    ui->labelUsername->setText(QString("Logged as ").append(QString(AppMess::username.c_str())));
    ui->listUsers->setMovement(QListView::Static);
    /*
    QTimer* timerReloadUsers = new QTimer();
    timerReloadUsers->start(2000);
    connect(timerReloadUsers, SIGNAL(timeout()), this, SLOT(reloadWindowLines()));*/

    reloadUsersList();
    this->update();

    QTimer* timerUpdateChat = new QTimer();
    timerUpdateChat->start(1500);
    connect(timerUpdateChat, SIGNAL(timeout()), this, SLOT(reloadChat()));

}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::on_btnSend_clicked()
{
    ui->labelUsername->setText(AppMess::username.c_str());

    char* msg = (char*) malloc(256);
    memset(msg, 0, 256);

    strcpy(msg, "sendMessage");
    write(AppMess::client->sockDescr, msg, 256);

    strcpy(msg, AppMess::username.c_str());
    write(AppMess::client->sockDescr, msg, 256);

    strcpy(msg, receiver.toStdString().c_str());

    if (receiver.contains("online"))
        strcpy(msg, receiver.toStdString().c_str()+strlen("[Online] "));

    write(AppMess::client->sockDescr, msg, 256);

    sprintf(msg, "From %s to %s : %s\n", AppMess::username.c_str(),
                                        receiver.toStdString().c_str(),
                                        ui->leUserInput->text().toStdString().c_str());
    write(AppMess::client->sockDescr, msg, 256);
    reloadChat();
    free(msg);
}
void ChatWindow::reloadUsersList()
{
    ui->listUsers->clear();
    char* msg = (char*) malloc(256);
    char* aux = (char*) malloc(256);
    memset(aux, 0, 256);
    // ONLINE
    printf("Getting online users!\n"); fflush(stdout);
    write(AppMess::client->sockDescr, "getUsersOnline", 256);
    read(AppMess::client->sockDescr, msg, 256);
    for ( ; 0 != strcmp(msg, "getUsersOnlineEnd"); read(AppMess::client->sockDescr, msg, 256))
    {
        sprintf(aux, "[Online] %s", msg);
        QString currUser(msg);
        int index = this->UserExistsAt(msg);
        if (-1 == index)
            ui->listUsers->addItem(new QListWidgetItem(QString(aux)));
        else
            ui->listUsers->item(index)->setText(QString("[Online] ").append(currUser));
        printf("1111%s1111\n", msg);
        fflush(stdout);

    }
    // ALL
    write(AppMess::client->sockDescr, "getUsersAll", 256);
    read(AppMess::client->sockDescr, msg, 256);
    for (; 0 != strcmp(msg, "getUsersAllEnd"); read(AppMess::client->sockDescr, msg, 256))
    {
        QString currUser(msg);
        QString onlineUser = QString("[Online] ").append(msg);
        if (-1 == this->UserExistsAt(currUser)) // NU ADAUG DE 2 ORI USERI CE SUNT ONLINE
            ui->listUsers->addItem(new QListWidgetItem(currUser));
    }
    free(msg);
    free(aux);
    ui->listUsers->setCurrentRow(0);
    receiver = ui->listUsers->item(0)->text();
    if (receiver.contains("Online"))
        receiver.remove(0, strlen("[Online] "));
    ui->labelPerson->setText(QString("Person ").append(receiver));
}
int ChatWindow::UserExistsAt(QString s)
{
   for (int i = 0; i < ui->listUsers->count(); i++)
       if (ui->listUsers->item(i)->text().contains(s))
           return i;
   return -1;
}

void ChatWindow::on_listUsers_itemClicked(QListWidgetItem *item)
{
    receiver = item->text();
    if (receiver.contains("Online"))
        receiver.remove(0, strlen("[Online] "));
    reloadChat();
}
void ChatWindow::reloadChat()
{
    char msg[256];
    memset(msg, 0, 256);
    write(AppMess::client->sockDescr, "getChatLines", 256);
    write(AppMess::client->sockDescr, receiver.toStdString().c_str(), 256);

    QString chatText("");
    read(AppMess::client->sockDescr, msg, 256);
    for (; 0 != strcmp(msg, "getChatLinesEnd"); read(AppMess::client->sockDescr, msg, 256))
    {
        strcat(msg, "\n");
        chatText.append(msg);
    }
    ui->textDiscussion->setText(chatText);
}

void ChatWindow::on_btnRefreshUsersList_clicked()
{
    reloadUsersList();
}
