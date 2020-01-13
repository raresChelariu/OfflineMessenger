#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    AppMess::client = new Client();
    AppMess::client->connect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnLogin_clicked()
{
    char* msg = (char*) malloc(256);
    memset(msg, 0, 256);
    sprintf(msg, "%s;%s", ui->leUsername->text().toStdString().c_str(), ui->lePassword->text().toStdString().c_str());
    write(AppMess::client->sockDescr, msg, 256);
    read(AppMess::client->sockDescr, msg, 256);
    if (!strcmp(msg, "LOGIN_YES"))
    {
        free(msg);
        AppMess::username = std::string(ui->leUsername->text().toStdString().c_str());
        ChatWindow* window = new ChatWindow();
        window->show();
        this->hide();
        return;
    }
    free(msg);
    this->setEnabled(false);
    QMessageBox* msgBox = new QMessageBox();
    msgBox->setText("Invalid credentials!");
    msgBox->exec();
    this->setEnabled(true);
}
